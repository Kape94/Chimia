#include "DiskOffloadableDataBuffer.h"

#include "Diagnostics.h"
#include "Random.h"

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>

//-----------------------------------------------------------------------------

USING_CHIMIA_CORE_NAMESPACE

//-----------------------------------------------------------------------------

namespace {
std::string
GenerateRandomString(const size_t size)
{
  constexpr auto characters =
    "abcdefghijklmnopqrstuvxzywABCDEFGHIJKLMNOPQRSTUVXZYW0123456789";
  const size_t charsSize = strlen(characters);

  std::string s(size, ' ');
  for (size_t i = 0; i < size; ++i) {
    const size_t randomIndex = (size_t)(Rand() * (float)charsSize);
    s[i] = characters[randomIndex];
  }
  return s;
}

const size_t FILE_NAME_LENGHT = 32;
}

//-----------------------------------------------------------------------------

DiskOffloadableDataBuffer::DiskOffloadableDataBuffer(
  const std::string& rootPath)
  : m_inMemoryBuffer()
  , m_rootPath(rootPath)
  , m_offloadedSizeInBytes(0)
  , m_fileNameInDisk("")
{
}

//-----------------------------------------------------------------------------

DiskOffloadableDataBuffer::DiskOffloadableDataBuffer(
  const size_t _maximumSize,
  const std::string& rootPath)
  : m_inMemoryBuffer(_maximumSize)
  , m_rootPath(rootPath)
  , m_offloadedSizeInBytes(0)
  , m_fileNameInDisk("")
{
}

//-----------------------------------------------------------------------------

DiskOffloadableDataBuffer::DiskOffloadableDataBuffer(
  DiskOffloadableDataBuffer&& other)
  : m_inMemoryBuffer(std::move(other.m_inMemoryBuffer))
  , m_rootPath(other.m_rootPath)
  , m_offloadedSizeInBytes(other.m_offloadedSizeInBytes)
  , m_fileNameInDisk(other.m_fileNameInDisk)
{
  other.m_rootPath = "";
  other.m_fileNameInDisk = "";
  other.m_offloadedSizeInBytes = 0;
}

//-----------------------------------------------------------------------------

DiskOffloadableDataBuffer&
DiskOffloadableDataBuffer::operator=(DiskOffloadableDataBuffer&& other)
{
  m_inMemoryBuffer = std::move(other.m_inMemoryBuffer);
  m_rootPath = other.m_rootPath;
  m_offloadedSizeInBytes = other.m_offloadedSizeInBytes;
  m_fileNameInDisk = other.m_fileNameInDisk;

  other.m_rootPath = "";
  other.m_fileNameInDisk = "";
  other.m_offloadedSizeInBytes = 0;

  return *this;
}

//-----------------------------------------------------------------------------

void
DiskOffloadableDataBuffer::Resize(const size_t size)
{
  m_inMemoryBuffer.Resize(size);
}

//-----------------------------------------------------------------------------

void
DiskOffloadableDataBuffer::Append(const RawDataView& dataView)
{
  m_inMemoryBuffer.Append(dataView);
}

//-----------------------------------------------------------------------------

void
DiskOffloadableDataBuffer::Append(const void* newData, const size_t newDataSize)
{
  m_inMemoryBuffer.Append(newData, newDataSize);
}

//-----------------------------------------------------------------------------

unsigned char*
DiskOffloadableDataBuffer::GetData()
{
  return m_inMemoryBuffer.GetData();
}

//-----------------------------------------------------------------------------

const unsigned char*
DiskOffloadableDataBuffer::GetData() const
{
  return m_inMemoryBuffer.GetData();
}

//-----------------------------------------------------------------------------

size_t
DiskOffloadableDataBuffer::GetSize() const
{
  return m_inMemoryBuffer.GetSize();
}

//-----------------------------------------------------------------------------

size_t
DiskOffloadableDataBuffer::GetAvailableSize() const
{
  return m_inMemoryBuffer.GetAvailableSize();
}

//-----------------------------------------------------------------------------

size_t
DiskOffloadableDataBuffer::GetMaximumSize() const
{
  return m_inMemoryBuffer.GetMaximumSize();
}

//-----------------------------------------------------------------------------

void
DiskOffloadableDataBuffer::Reset()
{
  m_inMemoryBuffer.Reset();
}

//-----------------------------------------------------------------------------

void
DiskOffloadableDataBuffer::Offload()
{
  if (IsOffloaded()) {
    return;
  }

  const std::string fileName = GenerateRandomString(FILE_NAME_LENGHT);

  const std::string separator = m_rootPath.back() == '/' ? "" : "/";
  const std::string filePath = m_rootPath + separator + fileName;

  std::ofstream outFile(filePath,
                        std::ios::binary | std::ios::out | std::ios::trunc);

  if (!outFile.is_open()) {
    Diagnostics::Error(1, "Failed to open file for disk offloading");
  }

  outFile.write((const char*)m_inMemoryBuffer.GetData(),
                m_inMemoryBuffer.GetSize());
  outFile.close();

  m_offloadedSizeInBytes = m_inMemoryBuffer.GetSize();
  m_fileNameInDisk = filePath;
  m_inMemoryBuffer.Delete();
}

//-----------------------------------------------------------------------------

void
DiskOffloadableDataBuffer::PullBack()
{
  if (!IsOffloaded()) {
    return;
  }

  std::ifstream inFile(m_fileNameInDisk, std::ios::binary | std::ios::in);
  if (!inFile.is_open()) {
    Diagnostics::Error(1, "Failed to open disk offloaded file for restoring");
  }

  std::vector<char> readBuffer(m_offloadedSizeInBytes);
  inFile.read(readBuffer.data(), m_offloadedSizeInBytes);
  inFile.close();
  std::remove(m_fileNameInDisk.c_str());

  m_inMemoryBuffer.Resize(m_offloadedSizeInBytes);
  m_inMemoryBuffer.Append(RawDataView{readBuffer.data(), m_offloadedSizeInBytes});
  m_fileNameInDisk = "";
  m_offloadedSizeInBytes = 0;
}

//-----------------------------------------------------------------------------

bool
DiskOffloadableDataBuffer::IsOffloaded() const
{
  return !m_fileNameInDisk.empty();
}

//-----------------------------------------------------------------------------
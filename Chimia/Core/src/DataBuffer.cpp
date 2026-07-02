#include "DataBuffer.h"

#include "CoreConstants.h"

#include <cstdlib>
#include <cstring>

//-----------------------------------------------------------------------------

USING_CHIMIA_CORE_NAMESPACE

//-----------------------------------------------------------------------------

DataBuffer::DataBuffer()
  : DataBuffer(0)
{
}

//-----------------------------------------------------------------------------

DataBuffer::DataBuffer(const size_t _maximumSize)
  : data(nullptr)
  , maximumSize(0)
  , currentSize(0)
{
  Resize(_maximumSize);
}

//-----------------------------------------------------------------------------

DataBuffer::DataBuffer(DataBuffer&& other)
  : data(other.data)
  , maximumSize(other.maximumSize)
  , currentSize(other.currentSize)
{
  other.data = nullptr;
  other.maximumSize = 0;
  other.currentSize = 0;
}

//-----------------------------------------------------------------------------

DataBuffer&
DataBuffer::operator=(DataBuffer&& other)
{
  data = other.data;
  maximumSize = other.maximumSize;
  currentSize = other.currentSize;

  other.data = nullptr;
  other.maximumSize = 0;
  other.currentSize = 0;

  return *this;
}

//-----------------------------------------------------------------------------

DataBuffer::~DataBuffer()
{
  Delete();
}

//-----------------------------------------------------------------------------

void
DataBuffer::Append(const RawDataView& dataView)
{
  Append(dataView.data, dataView.size);
}

//-----------------------------------------------------------------------------

void
DataBuffer::Append(const void* newData, const size_t newDataSize)
{
  const size_t remainingSize = RemainingSize();
  if (newDataSize > remainingSize) {
    const size_t sizeToIncrease = CalculateNewSize(newDataSize);
    IncreaseSize(sizeToIncrease);
    AppendData(newData, newDataSize);
  } else {
    AppendData(newData, newDataSize);
  }
}

//-----------------------------------------------------------------------------

size_t
DataBuffer::CalculateNewSize(const size_t incomingSize) const
{
  const size_t remainingSize = RemainingSize();
  const size_t neededSize = incomingSize - remainingSize;

  const float suggestedSizeFloat =
    CoreConstants::BUFFER_GROWTH_FACTOR * static_cast<float>(maximumSize);

  const size_t suggestedSize = static_cast<size_t>(suggestedSizeFloat);

  return std::max(neededSize, suggestedSize);
}

//-----------------------------------------------------------------------------

void
DataBuffer::IncreaseSize(const size_t size)
{
  const size_t newSize = maximumSize + size;
  Resize(newSize);
}

//-----------------------------------------------------------------------------

void
DataBuffer::Resize(const size_t size)
{
  if (size == 0) {
    return;
  } else if (size < maximumSize) {
    Delete();
  }

  if (data == nullptr) {
    data = Allocate(size);
  } else {
    unsigned char* newData = Allocate(size);
    memcpy(newData, data, currentSize);
    DeleteRawData();
    data = newData;
  }
  maximumSize = size;
}

//-----------------------------------------------------------------------------

unsigned char*
DataBuffer::Allocate(const size_t size) const
{
  unsigned char* allocated = static_cast<unsigned char*>(malloc(size));
  memset(allocated, 0, size);
  return allocated;
}

//-----------------------------------------------------------------------------

void
DataBuffer::Delete()
{
  DeleteRawData();
  currentSize = 0;
  maximumSize = 0;
}

//-----------------------------------------------------------------------------

void
DataBuffer::DeleteRawData()
{
  if (data != nullptr) {
    free(data);
    data = nullptr;
  }
}

//-----------------------------------------------------------------------------

size_t
DataBuffer::RemainingSize() const
{
  return maximumSize - currentSize;
}

//-----------------------------------------------------------------------------

void
DataBuffer::AppendData(const void* newData, const size_t newDataSize)
{
  memcpy(&data[currentSize], newData, newDataSize);
  currentSize += newDataSize;
}

//-----------------------------------------------------------------------------

void
DataBuffer::Reset()
{
  memset(data, 0, maximumSize);
  currentSize = 0;
}

//-----------------------------------------------------------------------------

unsigned char*
DataBuffer::GetData()
{
  return data;
}

//-----------------------------------------------------------------------------

const unsigned char*
DataBuffer::GetData() const
{
  return data;
}

//-----------------------------------------------------------------------------

size_t
DataBuffer::GetSize() const
{
  return currentSize;
}

//-----------------------------------------------------------------------------

size_t
DataBuffer::GetAvailableSize() const
{
  return maximumSize - currentSize;
}

//-----------------------------------------------------------------------------

size_t
DataBuffer::GetMaximumSize() const
{
  return maximumSize;
}

//-----------------------------------------------------------------------------
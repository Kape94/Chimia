#include "Buffer/RawBuffer.h"

#include <cstdlib>
#include <cstring>

//-----------------------------------------------------------------------------

USING_BITS_NAMESPACE

//-----------------------------------------------------------------------------

RawBuffer::RawBuffer()
  : RawBuffer(0)
{
}

//-----------------------------------------------------------------------------

RawBuffer::RawBuffer(const size_t _maximumSize)
  : data(nullptr)
  , maximumSize(0)
  , currentSize(0)
{
  Resize(_maximumSize);
}

//-----------------------------------------------------------------------------

RawBuffer::RawBuffer(RawBuffer&& other)
  : data(other.data)
  , maximumSize(other.maximumSize)
  , currentSize(other.currentSize)
{
  other.data = nullptr;
  other.maximumSize = 0;
  other.currentSize = 0;
}

//-----------------------------------------------------------------------------

RawBuffer&
RawBuffer::operator=(RawBuffer&& other)
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

RawBuffer::~RawBuffer()
{
  Delete();
}

//-----------------------------------------------------------------------------

void
RawBuffer::Append(const RawDataView& dataView)
{
  Append(dataView.data, dataView.size);
}

//-----------------------------------------------------------------------------

void
RawBuffer::Append(const void* newData, const size_t newDataSize)
{
  const size_t remainingSize = RemainingSize();
  if (newDataSize > remainingSize) {
    const size_t sizeToIncrease = newDataSize - remainingSize;
    IncreaseSize(sizeToIncrease);
    AppendData(newData, newDataSize);
  } else {
    AppendData(newData, newDataSize);
  }
}

//-----------------------------------------------------------------------------

void
RawBuffer::IncreaseSize(const size_t size)
{
  const size_t newSize = maximumSize + size;
  Resize(newSize);
}

//-----------------------------------------------------------------------------

void
RawBuffer::Resize(const size_t size)
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
RawBuffer::Allocate(const size_t size) const
{
  unsigned char* allocated = static_cast<unsigned char*>(malloc(size));
  memset(allocated, 0, size);
  return allocated;
}

//-----------------------------------------------------------------------------

void
RawBuffer::Delete()
{
  DeleteRawData();
  currentSize = 0;
  maximumSize = 0;
}

//-----------------------------------------------------------------------------

void
RawBuffer::DeleteRawData()
{
  if (data != nullptr) {
    free(data);
  }
}

//-----------------------------------------------------------------------------

size_t
RawBuffer::RemainingSize() const
{
  return maximumSize - currentSize;
}

//-----------------------------------------------------------------------------

void
RawBuffer::AppendData(const void* newData, const size_t newDataSize)
{
  memcpy(&data[currentSize], newData, newDataSize);
  currentSize += newDataSize;
}

//-----------------------------------------------------------------------------

void
RawBuffer::Reset()
{
  memset(data, 0, maximumSize);
  currentSize = 0;
}

//-----------------------------------------------------------------------------

unsigned char*
RawBuffer::GetData()
{
  return data;
}

//-----------------------------------------------------------------------------

size_t
RawBuffer::GetSize()
{
  return currentSize;
}

//-----------------------------------------------------------------------------

size_t
RawBuffer::GetAvailableSize()
{
  return maximumSize - currentSize;
}

//-----------------------------------------------------------------------------

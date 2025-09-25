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

RawBuffer::~RawBuffer()
{
  Clear();
}

//-----------------------------------------------------------------------------

void
RawBuffer::Append(void* newData, const size_t newDataSize)
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
  }

  if (data == nullptr) {
    data = Allocate(size);
  } else {
    unsigned char* newData = Allocate(size);
    memcpy(newData, data, currentSize);
    Clear();
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
RawBuffer::Clear()
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
RawBuffer::AppendData(void* newData, const size_t newDataSize)
{
  memcpy(&data[currentSize], newData, newDataSize);
  currentSize += newDataSize;
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

#include "Buffer/IndexedBuffer.h"

//-----------------------------------------------------------------------------

USING_MISC_NAMESPACE

//-----------------------------------------------------------------------------

IndexedBuffer::IndexedBuffer()
  : buffer()
{
}

//-----------------------------------------------------------------------------

IndexedBuffer::IndexedBuffer(const size_t maximumSize)
  : buffer(maximumSize)
{
}

//-----------------------------------------------------------------------------

void
IndexedBuffer::Append(void* data, const size_t dataSize)
{
  const size_t currentSize = buffer.GetSize();
  offsets.push_back(currentSize);

  buffer.Append(data, dataSize);
}

//-----------------------------------------------------------------------------

size_t
IndexedBuffer::NItems() const
{
  return offsets.size();
}

//-----------------------------------------------------------------------------

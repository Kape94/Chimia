#pragma once

#include <vector>

#include "Bits/Buffer/RawBuffer.h"
#include "Bits/Common/BitsNamespaceDefs.h"

BEGIN_BITS_NAMESPACE

class IndexedBuffer
{
public:
  IndexedBuffer();

  IndexedBuffer(const size_t maximumSize);

  IndexedBuffer(const IndexedBuffer& other) = delete;
  IndexedBuffer& operator=(const IndexedBuffer& other) = delete;

  IndexedBuffer(IndexedBuffer&& other) = delete;
  IndexedBuffer& operator=(IndexedBuffer&& other) = delete;

  void Append(void* data, const size_t dataSize);

  template<typename DataType>
  DataType& Get(const size_t i);

  size_t NItems() const;

private:
  std::vector<size_t> offsets;

  RawBuffer buffer;
};

//-----------------------------------------------------------------------------

template<typename DataType>
DataType&
IndexedBuffer::Get(const size_t i)
{
  const size_t correspondingOffset = offsets[i];
  unsigned char* data = buffer.GetData();

  unsigned char* indexedData = &data[correspondingOffset];
  return *reinterpret_cast<DataType*>(indexedData);
}

END_BITS_NAMESPACE

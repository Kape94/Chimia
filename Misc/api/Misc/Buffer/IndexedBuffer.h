#pragma once

#include <vector>

#include "Misc/Buffer/RawBuffer.h"
#include "Misc/Common/MiscNamespaceDefs.h"

BEGIN_MISC_NAMESPACE

class IndexedBuffer
{
public:
  IndexedBuffer();

  IndexedBuffer(const size_t maximumSize);

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

END_MISC_NAMESPACE

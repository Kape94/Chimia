#pragma once

#include <cstddef>

#include "Bits/Common/BitsNamespaceDefs.h"

BEGIN_BITS_NAMESPACE

class RawBuffer
{
public:
  RawBuffer();

  RawBuffer(const size_t _maximumSize);

  ~RawBuffer();

  void Append(const void* newData, const size_t newDataSize);

  unsigned char* GetData();
  size_t GetSize();
  size_t GetAvailableSize();

  void Reset();

private:
  void IncreaseSize(const size_t size);

  void Resize(const size_t size);

  unsigned char* Allocate(const size_t size) const;

  void Delete();

  size_t RemainingSize() const;

  void AppendData(const void* newData, const size_t newDataSize);

  size_t maximumSize = 0;
  size_t currentSize = 0;

  unsigned char* data = nullptr;
};

END_BITS_NAMESPACE

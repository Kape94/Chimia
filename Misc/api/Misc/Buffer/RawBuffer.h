#pragma once

#include <cstddef>

#include "Misc/Common/MiscNamespaceDefs.h"

BEGIN_MISC_NAMESPACE

class RawBuffer
{
public:
  RawBuffer();

  RawBuffer(const size_t _maximumSize);

  ~RawBuffer();

  void Append(void* newData, const size_t newDataSize);

  unsigned char* GetData();
  size_t GetSize();

private:
  void IncreaseSize(const size_t size);

  void Resize(const size_t size);

  unsigned char* Allocate(const size_t size) const;

  void Clear();

  size_t RemainingSize() const;

  void AppendData(void* newData, const size_t newDataSize);

  size_t maximumSize = 0;
  size_t currentSize = 0;

  unsigned char* data = nullptr;
};

END_MISC_NAMESPACE

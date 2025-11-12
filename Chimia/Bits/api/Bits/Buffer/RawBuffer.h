#pragma once

#include "Bits/Common/BitsNamespaceDefs.h"

#include "Core/Types.h"

#include <cstddef>

BEGIN_BITS_NAMESPACE

class RawBuffer
{
public:
  RawBuffer();

  RawBuffer(const size_t _maximumSize);

  RawBuffer(RawBuffer&& other);
  RawBuffer& operator=(RawBuffer&& other);

  RawBuffer(const RawBuffer& other) = delete;
  RawBuffer& operator=(const RawBuffer& other) = delete;

  ~RawBuffer();

  void Resize(const size_t size);

  void Append(const RawDataView& dataView);
  void Append(const void* newData, const size_t newDataSize);

  unsigned char* GetData();
  const unsigned char* GetData() const;

  size_t GetSize() const;
  size_t GetAvailableSize() const;

  void Reset();

private:
  void IncreaseSize(const size_t size);

  unsigned char* Allocate(const size_t size) const;

  void Delete();
  void DeleteRawData();

  size_t RemainingSize() const;

  void AppendData(const void* newData, const size_t newDataSize);

  size_t maximumSize = 0;
  size_t currentSize = 0;

  unsigned char* data = nullptr;
};

END_BITS_NAMESPACE

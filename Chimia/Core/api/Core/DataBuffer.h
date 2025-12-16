#pragma once

#include "Core/Types.h"
#include "CoreNamespaceDefs.h"

#include <cstddef>

BEGIN_CHIMIA_CORE_NAMESPACE

class DataBuffer
{
public:
  DataBuffer();

  DataBuffer(const size_t _maximumSize);

  DataBuffer(DataBuffer&& other);
  DataBuffer& operator=(DataBuffer&& other);

  DataBuffer(const DataBuffer& other) = delete;
  DataBuffer& operator=(const DataBuffer& other) = delete;

  ~DataBuffer();

  void Resize(const size_t size);

  void Append(const RawDataView& dataView);
  void Append(const void* newData, const size_t newDataSize);

  unsigned char* GetData();
  const unsigned char* GetData() const;

  size_t GetSize() const;
  size_t GetAvailableSize() const;
  size_t GetMaximumSize() const;

  void Reset();
  void Delete();

private:
  void IncreaseSize(const size_t size);

  unsigned char* Allocate(const size_t size) const;

  void DeleteRawData();

  size_t RemainingSize() const;

  void AppendData(const void* newData, const size_t newDataSize);

  size_t maximumSize = 0;
  size_t currentSize = 0;

  unsigned char* data = nullptr;
};

END_CHIMIA_CORE_NAMESPACE

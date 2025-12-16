#pragma once

#include "CoreNamespaceDefs.h"
#include "DataBuffer.h"

#include <cstddef>
#include <string>

BEGIN_CHIMIA_CORE_NAMESPACE

class DiskOffloadableDataBuffer
{
public:
  DiskOffloadableDataBuffer(const std::string& rootPath);

  DiskOffloadableDataBuffer(const size_t _maximumSize,
                            const std::string& rootPath);

  DiskOffloadableDataBuffer(DiskOffloadableDataBuffer&& other);
  DiskOffloadableDataBuffer& operator=(DiskOffloadableDataBuffer&& other);

  DiskOffloadableDataBuffer(const DiskOffloadableDataBuffer& other) = delete;
  DiskOffloadableDataBuffer& operator=(const DiskOffloadableDataBuffer& other) =
    delete;

  ~DiskOffloadableDataBuffer() = default;

  void Resize(const size_t size);

  void Append(const RawDataView& dataView);
  void Append(const void* newData, const size_t newDataSize);

  unsigned char* GetData();
  const unsigned char* GetData() const;

  size_t GetSize() const;
  size_t GetAvailableSize() const;
  size_t GetMaximumSize() const;

  void Reset();

  void Offload();
  void PullBack();

private:
  bool IsOffloaded() const;

  std::string m_fileNameInDisk;
  size_t m_offloadedSizeInBytes = 0;

  DataBuffer m_inMemoryBuffer;
  std::string m_rootPath;
};

END_CHIMIA_CORE_NAMESPACE

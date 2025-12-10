#include "BatchUtils.h"
#include "Core/Types.h"
#include <cstddef>
#include <numeric>

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

std::vector<size_t>
BatchUtils::ComputeBatchingOffsetsForSize(const size_t totalSize,
                                          const size_t batchSize)
{
  std::vector<size_t> batchOffsets;

  for (size_t offset = 0; offset < totalSize; offset += batchSize) {
    batchOffsets.push_back(offset);
  }
  batchOffsets.push_back(totalSize);

  return batchOffsets;
}

// ----------------------------------------------------------------------------

void
BatchUtils::ForEachBatchRange(const size_t totalSize,
                              const size_t batchSize,
                              const std::function<void(size_t, size_t)>& action)
{
  const std::vector<size_t> offsets =
    ComputeBatchingOffsetsForSize(totalSize, batchSize);

  for (size_t i = 1; i < offsets.size(); ++i) {
    const size_t rangeStart = offsets[i - 1];
    const size_t rangeEnd = offsets[i];
    const size_t rangeSize = rangeEnd - rangeStart;

    action(rangeStart, rangeSize);
  }
}

// ----------------------------------------------------------------------------

size_t
BatchUtils::TotalDataSize(const std::initializer_list<RawDataView>& dataViews)
{
  static auto accumulateDataSizeFn = [](const size_t current,
                                        const RawDataView& data) {
    return current + data.size;
  };

  return std::accumulate(
    dataViews.begin(), dataViews.end(), 0, accumulateDataSizeFn);
}

// ----------------------------------------------------------------------------
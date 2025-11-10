#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include <cstddef>
#include <functional>
#include <vector>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace BatchUtils {
std::vector<size_t>
ComputeBatchingOffsetsForSize(const size_t totalSize, const size_t batchSize);

void
ForEachBatchRange(const size_t totalSize,
                  const size_t batchSize,
                  const std::function<void(size_t, size_t)>& action);
}

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
#pragma once

// ----------------------------------------------------------------------------

#include "eImmediateFlushingPolicy.h"

#include "Core/DataBuffer.h"
#include "Core/Types.h"
#include "Draw3DNamespaceDefs.h"
#include "Rendering/InstancedRenderAction.h"
#include "Rendering/RenderAction.h"

#include <cstddef>
#include <functional>
#include <initializer_list>
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

void
RenderByBatches(const size_t totalSize,
                const size_t batchSize,
                const DataBuffer& cpuBuffer,
                Rendering::RenderAction& gpuAction);

void
RenderInstancedByBatches(
  const size_t totalSize,
  const size_t batchSize,
  const size_t instanceSize,
  const DataBuffer& cpuBuffer,
  std::vector<Rendering::InstancedRenderAction>& gpuActions);

size_t
TotalDataSize(const std::initializer_list<RawDataView>& dataViews);

bool
ShouldKeepInput(const eImmediateFlusingPolicy policy);
}

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
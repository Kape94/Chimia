#pragma once

// ----------------------------------------------------------------------------

#include "BatchUtils.h"
#include "Draw3DNamespaceDefs.h"

#include "DataBindingProvider.h"
#include "Transition.h"
#include "eImmediateFlushingPolicy.h"

#include "Core/ClassDefs.h"
#include "Core/DataBuffer.h"
#include "Types.h"

#include <functional>
#include <initializer_list>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class ImmediateTransitionsBatch
{
public:
  DEFAULT_CONSTUCTIBLE(ImmediateTransitionsBatch)
  NON_COPYABLE_NON_MOVABLE(ImmediateTransitionsBatch)

  void Create(const Transition& transition,
              const BatchingSettings& batchingSettings,
              const DataBindingProvider& dataBindings,
              const std::function<void(void)>& onFlush);

  void Draw(const RawDataView& instanceData);
  void Draw(const std::initializer_list<RawDataView>& instanceDatas);

  void Flush(const eImmediateFlusingPolicy flushingPolicy);

private:
  void DoFlush(const eImmediateFlusingPolicy flushingPolicy);

  void HandleDynamicResizing();

  void ResizeBatch(const size_t batchSize);

  size_t CurrentGPUBatchSize() const;

  // Fixed attributes, not changed after initial creation
  std::function<void(void)> m_onFlush;
  BatchingSettings m_batchingSettings;
  size_t m_instancedDataSizeInBytes = 0;

  // This attribute only gets changed when a buffer resize happens
  size_t m_currentGPUBatchSizeInBytes = 0;

  // These attributes change often, during frame flow
  DataBuffer m_instancedInputBuffer;

  std::vector<BatchUtils::InstancedGPUComponent> m_gpuComponents;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
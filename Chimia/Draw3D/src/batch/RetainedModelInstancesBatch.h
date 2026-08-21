#pragma once

// ----------------------------------------------------------------------------

#include "BatchUtils.h"
#include "Core/ObjectRegistry.h"
#include "DataBindingProvider.h"
#include "Draw3DNamespaceDefs.h"
#include "Model.h"

#include "Core/ClassDefs.h"
#include "Core/DataBuffer.h"
#include "Types.h"

#include <functional>
#include <initializer_list>
#include <vector>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class RetainedModelInstancesBatch
{
public:
  DEFAULT_CONSTUCTIBLE(RetainedModelInstancesBatch)
  NON_COPYABLE_NON_MOVABLE(RetainedModelInstancesBatch)

  void Create(const Model& model,
              const BatchingSettings& batchingSettings,
              const DataBindingProvider& dataBindings,
              const std::function<void()>& onRender);

  unsigned AddInstance(const RawDataView& instanceData);
  unsigned AddInstance(const std::initializer_list<RawDataView>& instanceDatas);

  void DeleteInstance(unsigned instanceID);

  void Render();

private:
  void CreateGPUBuffers(const Model& model,
                        const size_t batchSize,
                        const size_t instanceBatchDataSize,
                        const DataBindingProvider& dataBindings);

  bool HasSomethingToRender() const;

  bool CanRenderWithCurrentBuffers() const;

  void RebuildInputBuffer();

  void HandleDynamicResizing();

  void ResizeBatch(const size_t batchSize);

  void RenderByBatches();

  void RenderCurrentBuffers();

  size_t CurrentGPUBatchSizeInBytes() const;

  // Fixed attributes, don't get changed after initial batch creation
  std::function<void()> m_onRender;
  BatchingSettings m_batchingSettings;
  size_t m_instanceDataSizeInBytes = 0;

  // Cache attribute to indicate whenever a new instance gets added
  bool m_shouldRebuildBuffers = true;
  size_t m_currentGPUBatchSize = 0;

  // The attributes get modified often during rendering
  DataBuffer m_instanceDataBuffer;
  ObjectRegistry<DataBuffer> m_instanceTable;
  std::vector<BatchUtils::InstancedGPUComponent> m_gpuComponent;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
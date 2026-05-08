#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "Model.h"
#include "ObjectTable.h"

#include "Core/ClassDefs.h"
#include "Core/DataBuffer.h"
#include "Rendering/InstancedBuffer.h"
#include "Rendering/ShaderAttribute.h"
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
              const Rendering::ShaderAttributes& vertexAttributes,
              const Rendering::ShaderAttributes& instanceAttributes,
              const std::function<void()>& onRender);

  unsigned AddInstance(const RawDataView& instanceData);
  unsigned AddInstance(const std::initializer_list<RawDataView>& instanceDatas);

  void DeleteInstance(unsigned instanceID);

  void Render();

private:
  void CreateGPUBuffers(const Model& model,
                        const size_t batchSize,
                        const size_t instanceBatchDataSize,
                        const Rendering::ShaderAttributes& vertexAttributes,
                        const Rendering::ShaderAttributes& instanceAttributes);

  bool HasSomethingToRender() const;

  bool CanRenderWithCurrentBuffers() const;

  void RebuildInputBuffer();

  void HandleDynamicResizing();

  void ResizeBatch(const size_t batchSize);

  void RenderByBatches();

  void HandleRenderingForBatchRange(const size_t rangeStart,
                                    const size_t rangeSize);

  void LoadBatchAndRender(const void* instancesData, const unsigned nInstances);

  void RenderCurrentBuffers();

  size_t CurrentGPUBatchSizeInBytes() const;

  // Fixed attributes, don't get changed after initial batch creation
  std::function<void()> m_onRender;
  BatchingSettings m_batchingSettings;
  Rendering::ShaderAttributes m_instancedAttributes;
  size_t m_instanceDataSizeInBytes = 0;

  // Cache attribute to indicate whenever a new instance gets added
  bool m_shouldRebuildBuffers = true;
  size_t m_currentGPUBatchSize = 0;

  // The attributes get modified often during rendering
  DataBuffer m_instanceDataBuffer;
  ObjectTable<DataBuffer> m_instanceTable;
  std::vector<Rendering::InstancedBuffer> m_gpuBuffers;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
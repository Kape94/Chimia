#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "Model.h"

#include "Core/ClassDefs.h"
#include "Core/DataBuffer.h"
#include "Rendering/InstancedBuffer.h"
#include "Rendering/ReusableIndexedVertexBufferObject.h"
#include "Rendering/ShaderAttribute.h"
#include "Types.h"

#include <functional>
#include <initializer_list>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class ModelBatch
{
public:
  DEFAULT_CONSTUCTIBLE(ModelBatch)
  NON_COPYABLE_NON_MOVABLE(ModelBatch)

  void Create(const Model& model,
              const BatchingSettings& batchingSettings,
              const Rendering::ShaderAttributes& vertexAttributes,
              const Rendering::ShaderAttributes& instanceAttributes,
              const std::function<void(void)>& onFlush);

  void Draw(const RawDataView& instanceData);
  void Draw(const std::initializer_list<RawDataView>& instanceDatas);

  void Flush();

private:
  void AddGPUBuffer(
    const Rendering::ReusableIndexedVertexBufferObject& bufferData,
    const size_t instanceBatchSize,
    const Rendering::ShaderAttributes& vertexAttributes,
    const Rendering::ShaderAttributes& instanceAttributes);

  void HandleFlushByDemand(const size_t incomingSizeInBytes);

  void DoFlush();

  void HandleDynamicResizing();

  void ResizeBatch(const size_t batchSize);

  size_t CurrentBatchSize() const;

  // Fixed attributes, not changed after initial creation
  std::function<void(void)> m_onFlush;
  BatchingSettings m_batchingSettings;
  Rendering::ShaderAttributes m_instancedAttributes;
  size_t m_instancedDataSizeInBytes = 0;

  // This attribute only gets changed when a flush by demand happens
  size_t m_nInstancesFlushedByDemand = 0;

  // These attributes change often, during frame flow
  DataBuffer m_instancedInputBuffer;
  std::vector<Rendering::InstancedBuffer> m_gpuBuffers;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
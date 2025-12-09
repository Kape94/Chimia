#pragma once

#include "Draw3DNamespaceDefs.h"
#include "Model.h"

#include "Core/DataBuffer.h"
#include "Rendering/InstancedBuffer.h"
#include "Rendering/ReusableIndexedVertexBufferObject.h"
#include "Rendering/ShaderAttribute.h"
#include "Types.h"

#include <functional>
#include <initializer_list>

BEGIN_CHIMIA_DRAW3D_NAMESPACE

class ModelBatch
{
public:
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

  void HandleFlushByDemand();

  std::function<void(void)> m_onFlush;
  BatchingSettings m_batchingSettings;

  size_t m_instancedDataSize = 0;
  DataBuffer m_instancedInputBuffer;
  std::vector<Rendering::InstancedBuffer> m_gpuBuffers;
};

END_CHIMIA_DRAW3D_NAMESPACE
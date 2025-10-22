#pragma once

#include "BufferData.h"
#include "Draw3DNamespaceDefs.h"

#include "Bits/Buffer/RawBuffer.h"
#include "Bits/Buffer/RawDataView.h"
#include "Rendering/InstancedBuffer.h"
#include "Rendering/ShaderAttribute.h"

#include <functional>
#include <initializer_list>

BEGIN_CHIMIA_DRAW3D_NAMESPACE

class ModelBatch
{
public:
  void Create(const BufferData& bufferData,
              const size_t instanceBatchSize,
              const Rendering::ShaderAttributes& vertexAttributes,
              const Rendering::ShaderAttributes& instanceAttributes,
              const std::function<void(void)>& onFlush);

  void Create(const std::vector<BufferData>& bufferDatas,
              const size_t instanceBatchSize,
              const Rendering::ShaderAttributes& vertexAttributes,
              const Rendering::ShaderAttributes& instanceAttributes,
              const std::function<void(void)>& onFlush);

  void Draw(const Bits::RawDataView& instanceData);
  void Draw(const std::initializer_list<Bits::RawDataView>& instanceDatas);

  void Flush();

private:
  void AddGPUBuffer(const BufferData& bufferData,
                    const size_t instanceBatchSize,
                    const Rendering::ShaderAttributes& vertexAttributes,
                    const Rendering::ShaderAttributes& instanceAttributes);

  size_t CalculateInstancedDataSize(
    const Rendering::ShaderAttributes& instancedDataAttributes);
  void HandleFlushByDemand();

  std::function<void(void)> m_onFlush;

  size_t m_instancedDataSize = 0;
  Bits::RawBuffer m_instancedInputBuffer;
  std::vector<Rendering::InstancedBuffer> m_gpuBuffers;
};

END_CHIMIA_DRAW3D_NAMESPACE
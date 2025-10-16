#include "ModelBatch.h"
#include "Rendering/InstancedBuffer.h"
#include "Rendering/ShaderAttribute.h"
#include <numeric>

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

void
ModelBatch::Create(const BufferData& bufferData,
                   const size_t dataSizePerInstance,
                   const size_t instanceBatchSize,
                   const Rendering::ShaderAttributes& vertexAttributes,
                   const Rendering::ShaderAttributes& instanceAttributes,
                   const std::function<void(void)>& onFlush)
{
  m_onFlush = onFlush;
  m_instanceInputSize = dataSizePerInstance;

  AddGPUBuffer(bufferData,
               dataSizePerInstance,
               instanceBatchSize,
               vertexAttributes,
               instanceAttributes);

  m_instancedInputBuffer.Resize(instanceBatchSize * dataSizePerInstance);
}

// ----------------------------------------------------------------------------

void
ModelBatch::Create(const std::vector<BufferData>& bufferDatas,
                   const size_t dataSizePerInstance,
                   const size_t instanceBatchSize,
                   const Rendering::ShaderAttributes& vertexAttributes,
                   const Rendering::ShaderAttributes& instanceAttributes,
                   const std::function<void(void)>& onFlush)
{
  m_onFlush = onFlush;
  m_instanceInputSize = dataSizePerInstance;

  for (auto& bufferData : bufferDatas) {
    AddGPUBuffer(bufferData,
                 dataSizePerInstance,
                 instanceBatchSize,
                 vertexAttributes,
                 instanceAttributes);
  }

  m_instancedInputBuffer.Resize(instanceBatchSize * dataSizePerInstance);
}

// ----------------------------------------------------------------------------

void
ModelBatch::AddGPUBuffer(const BufferData& bufferData,
                         const size_t dataSizePerInstance,
                         const size_t instanceBatchSize,
                         const Rendering::ShaderAttributes& vertexAttributes,
                         const Rendering::ShaderAttributes& instanceAttributes)
{
  Rendering::InstancedBuffer& inserted = m_gpuBuffers.emplace_back();
  inserted.CreateInstanced(bufferData.VertexData().data(),
                           bufferData.VertexData().size(),
                           bufferData.Indices().data(),
                           bufferData.Indices().size(),
                           vertexAttributes,
                           nullptr,
                           dataSizePerInstance,
                           instanceBatchSize,
                           instanceAttributes);
}

// ----------------------------------------------------------------------------

void
ModelBatch::Draw(const Bits::RawDataView& instanceData)
{
  if (m_instancedInputBuffer.GetAvailableSize() < instanceData.size) {
    m_onFlush();
    Flush();
  }

  m_instancedInputBuffer.Append(instanceData);
}

// ----------------------------------------------------------------------------

void
ModelBatch::Draw(const std::initializer_list<Bits::RawDataView>& instanceDatas)
{
  // TODO: cache instanceData size
  const size_t totalIncomingSize = std::accumulate(
    instanceDatas.begin(),
    instanceDatas.end(),
    0,
    [](size_t a, const Bits::RawDataView& b) { return a + b.size; });

  if (m_instancedInputBuffer.GetAvailableSize() < totalIncomingSize) {
    m_onFlush();
    Flush();
  }

  for (const auto& instanceData : instanceDatas) {
    m_instancedInputBuffer.Append(instanceData);
  }
}

// ----------------------------------------------------------------------------

void
ModelBatch::Flush()
{
  const size_t totalInputSize = m_instancedInputBuffer.GetSize();
  if (totalInputSize == 0) {
    return;
  }

  m_onFlush();

  const size_t nInstances = totalInputSize / m_instanceInputSize;
  for (auto& buffer : m_gpuBuffers) {
    buffer.LoadInstancedData(
      m_instancedInputBuffer.GetData(), m_instanceInputSize, nInstances);

    buffer.Render();
  }

  m_instancedInputBuffer.Reset();
}

// ----------------------------------------------------------------------------
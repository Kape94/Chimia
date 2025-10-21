#include "ModelBatch.h"
#include "Rendering/InstancedBuffer.h"
#include "Rendering/ShaderAttribute.h"
#include <numeric>

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

void
ModelBatch::Create(const BufferData& bufferData,
                   const size_t instanceBatchSize,
                   const Rendering::ShaderAttributes& vertexAttributes,
                   const Rendering::ShaderAttributes& instanceAttributes,
                   const std::function<void(void)>& onFlush)
{
  m_onFlush = onFlush;
  m_instancedDataSize = CalculateInstancedDataSize(instanceAttributes);

  AddGPUBuffer(
    bufferData, instanceBatchSize, vertexAttributes, instanceAttributes);

  m_instancedInputBuffer.Resize(instanceBatchSize * m_instancedDataSize);
}

// ----------------------------------------------------------------------------

void
ModelBatch::Create(const std::vector<BufferData>& bufferDatas,
                   const size_t instanceBatchSize,
                   const Rendering::ShaderAttributes& vertexAttributes,
                   const Rendering::ShaderAttributes& instanceAttributes,
                   const std::function<void(void)>& onFlush)
{
  m_onFlush = onFlush;
  m_instancedDataSize = CalculateInstancedDataSize(instanceAttributes);

  for (auto& bufferData : bufferDatas) {
    AddGPUBuffer(
      bufferData, instanceBatchSize, vertexAttributes, instanceAttributes);
  }

  m_instancedInputBuffer.Resize(instanceBatchSize * m_instancedDataSize);
}

// ----------------------------------------------------------------------------

size_t
ModelBatch::CalculateInstancedDataSize(
  const Rendering::ShaderAttributes& instancedDataAttributes)
{
  return std::accumulate(
    instancedDataAttributes.begin(),
    instancedDataAttributes.end(),
    0,
    [](size_t current, const Rendering::ShaderAttribute& attr) {
      return current + attr.DataSizeInBytes();
    });
}

// ----------------------------------------------------------------------------

void
ModelBatch::AddGPUBuffer(const BufferData& bufferData,
                         const size_t instanceBatchSize,
                         const Rendering::ShaderAttributes& vertexAttributes,
                         const Rendering::ShaderAttributes& instanceAttributes)
{
  Rendering::InstancedBuffer& inserted = m_gpuBuffers.emplace_back();
  inserted.CreateInstanced(bufferData.VertexData().data(),
                           bufferData.VertexData().size() * sizeof(float),
                           bufferData.Indices().data(),
                           bufferData.Indices().size(),
                           vertexAttributes,
                           nullptr,
                           m_instancedDataSize,
                           instanceBatchSize,
                           instanceAttributes);
}

// ----------------------------------------------------------------------------

void
ModelBatch::Draw(const Bits::RawDataView& instanceData)
{
  HandleFlushByDemand();

  m_instancedInputBuffer.Append(instanceData);
}

// ----------------------------------------------------------------------------

void
ModelBatch::Draw(const std::initializer_list<Bits::RawDataView>& instanceDatas)
{
  HandleFlushByDemand();

  for (const auto& instanceData : instanceDatas) {
    m_instancedInputBuffer.Append(instanceData);
  }
}

// ----------------------------------------------------------------------------

void
ModelBatch::HandleFlushByDemand()
{
  if (m_instancedInputBuffer.GetAvailableSize() < m_instancedDataSize) {
    m_onFlush();
    Flush();
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

  const size_t nInstances = totalInputSize / m_instancedDataSize;
  for (auto& buffer : m_gpuBuffers) {
    buffer.LoadInstancedData(
      m_instancedInputBuffer.GetData(), m_instancedDataSize, nInstances);

    buffer.Render();
  }

  m_instancedInputBuffer.Reset();
}

// ----------------------------------------------------------------------------
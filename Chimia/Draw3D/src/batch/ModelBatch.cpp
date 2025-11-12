#include "ModelBatch.h"
#include "Core/Types.h"
#include "Rendering/InstancedBuffer.h"
#include "Rendering/ReusableIndexedVertexBufferObject.h"
#include "Rendering/ShaderAttribute.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

void
ModelBatch::Create(const Model& model,
                   const size_t instanceBatchSize,
                   const Rendering::ShaderAttributes& vertexAttributes,
                   const Rendering::ShaderAttributes& instanceAttributes,
                   const std::function<void(void)>& onFlush)
{
  m_onFlush = onFlush;
  m_instancedDataSize = instanceAttributes.ComputeTotalSizeOfAttributes();

  model.ForEachBuffer(
    [&](const Rendering::ReusableIndexedVertexBufferObject& reusableBuffer) {
      AddGPUBuffer(reusableBuffer,
                   instanceBatchSize,
                   vertexAttributes,
                   instanceAttributes);
    });

  m_instancedInputBuffer.Resize(instanceBatchSize * m_instancedDataSize);
}

// ----------------------------------------------------------------------------

void
ModelBatch::AddGPUBuffer(
  const Rendering::ReusableIndexedVertexBufferObject& bufferData,
  const size_t instanceBatchSize,
  const Rendering::ShaderAttributes& vertexAttributes,
  const Rendering::ShaderAttributes& instanceAttributes)
{
  Rendering::InstancedBuffer& inserted = m_gpuBuffers.emplace_back();
  inserted.CreateInstanced(
    bufferData,
    vertexAttributes,
    RawArrayView{ nullptr, instanceBatchSize, m_instancedDataSize },
    instanceAttributes);
}

// ----------------------------------------------------------------------------

void
ModelBatch::Draw(const RawDataView& instanceData)
{
  HandleFlushByDemand();

  m_instancedInputBuffer.Append(instanceData);
}

// ----------------------------------------------------------------------------

void
ModelBatch::Draw(const std::initializer_list<RawDataView>& instanceDatas)
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
    buffer.LoadInstancedData(RawArrayView{
      m_instancedInputBuffer.GetData(), nInstances, m_instancedDataSize });

    buffer.Render();
  }

  m_instancedInputBuffer.Reset();
}

// ----------------------------------------------------------------------------
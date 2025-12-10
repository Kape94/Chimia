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
                   const BatchingSettings& batchingSettings,
                   const Rendering::ShaderAttributes& vertexAttributes,
                   const Rendering::ShaderAttributes& instanceAttributes,
                   const std::function<void(void)>& onFlush)
{
  m_onFlush = onFlush;
  m_batchingSettings = batchingSettings;
  m_instancedDataSizeInBytes =
    instanceAttributes.ComputeTotalSizeOfAttributes();

  const size_t batchSize = batchingSettings.initialBatchSize;
  model.ForEachBuffer(
    [&](const Rendering::ReusableIndexedVertexBufferObject& reusableBuffer) {
      AddGPUBuffer(
        reusableBuffer, batchSize, vertexAttributes, instanceAttributes);
    });

  m_instancedInputBuffer.Resize(batchSize * m_instancedDataSizeInBytes);
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
    RawArrayView{ nullptr, instanceBatchSize, m_instancedDataSizeInBytes },
    instanceAttributes);
}

// ----------------------------------------------------------------------------

void
ModelBatch::Draw(const RawDataView& instanceData)
{
  HandleFlushByDemand(instanceData.size);

  m_instancedInputBuffer.Append(instanceData);
}

// ----------------------------------------------------------------------------

void
ModelBatch::Draw(const std::initializer_list<RawDataView>& instanceDatas)
{
  for (const auto& instanceData : instanceDatas) {
    Draw(instanceData);
  }
}

// ----------------------------------------------------------------------------

void
ModelBatch::HandleFlushByDemand(const size_t incomingSizeInBytes)
{
  if (m_instancedInputBuffer.GetAvailableSize() < incomingSizeInBytes) {
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

  const size_t nInstances = totalInputSize / m_instancedDataSizeInBytes;
  for (auto& buffer : m_gpuBuffers) {
    buffer.LoadInstancedData(RawArrayView{ m_instancedInputBuffer.GetData(),
                                           nInstances,
                                           m_instancedDataSizeInBytes });

    buffer.Render();
  }

  m_instancedInputBuffer.Reset();
}

// ----------------------------------------------------------------------------

size_t
ModelBatch::CurrentBatchSize() const
{
  return m_instancedInputBuffer.GetMaximumSize() / m_instancedDataSizeInBytes;
}

// ----------------------------------------------------------------------------
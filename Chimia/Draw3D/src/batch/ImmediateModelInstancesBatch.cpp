#include "ImmediateModelInstancesBatch.h"

#include "Core/Types.h"
#include "Rendering/InstancedBuffer.h"
#include "Rendering/ReusableIndexedVertexBufferObject.h"
#include "Rendering/ShaderAttribute.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

void
ImmediateModelInstancesBatch::Create(
  const Model& model,
  const BatchingSettings& batchingSettings,
  const Rendering::ShaderAttributes& vertexAttributes,
  const Rendering::ShaderAttributes& instanceAttributes,
  const std::function<void(void)>& onFlush)
{
  m_onFlush = onFlush;
  m_batchingSettings = batchingSettings;
  m_instancedAttributes = instanceAttributes;
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
ImmediateModelInstancesBatch::AddGPUBuffer(
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
ImmediateModelInstancesBatch::Draw(const RawDataView& instanceData)
{
  HandleFlushByDemand(instanceData.size);

  m_instancedInputBuffer.Append(instanceData);
}

// ----------------------------------------------------------------------------

void
ImmediateModelInstancesBatch::Draw(
  const std::initializer_list<RawDataView>& instanceDatas)
{
  for (const auto& instanceData : instanceDatas) {
    Draw(instanceData);
  }
}

// ----------------------------------------------------------------------------

void
ImmediateModelInstancesBatch::HandleFlushByDemand(
  const size_t incomingSizeInBytes)
{
  if (m_instancedInputBuffer.GetAvailableSize() < incomingSizeInBytes) {
    const size_t nInstancesInBuffer =
      m_instancedInputBuffer.GetSize() / m_instancedDataSizeInBytes;
    m_nInstancesFlushedByDemand += nInstancesInBuffer;

    DoFlush();
  }
}

// ----------------------------------------------------------------------------

void
ImmediateModelInstancesBatch::Flush()
{
  const size_t totalInputSize = m_instancedInputBuffer.GetSize();
  if (totalInputSize == 0) {
    return;
  }

  DoFlush();
  HandleDynamicResizing();
}

// ----------------------------------------------------------------------------

void
ImmediateModelInstancesBatch::DoFlush()
{
  m_onFlush();

  const size_t nInstances =
    m_instancedInputBuffer.GetSize() / m_instancedDataSizeInBytes;
  for (auto& buffer : m_gpuBuffers) {
    buffer.LoadInstancedData(RawArrayView{ m_instancedInputBuffer.GetData(),
                                           nInstances,
                                           m_instancedDataSizeInBytes });

    buffer.Render();
  }

  m_instancedInputBuffer.Reset();
}

// ----------------------------------------------------------------------------

void
ImmediateModelInstancesBatch::HandleDynamicResizing()
{
  if (m_nInstancesFlushedByDemand == 0) {
    return;
  }

  const size_t maximumAllowed = m_batchingSettings.maximumBatchSize;
  const size_t batchSize = CurrentBatchSize();
  if (batchSize < maximumAllowed) {
    const size_t desiredBatchSize = batchSize + m_nInstancesFlushedByDemand;
    const size_t newBatchSize = std::min(desiredBatchSize, maximumAllowed);

    ResizeBatch(newBatchSize);
  }

  m_nInstancesFlushedByDemand = 0;
}

// ----------------------------------------------------------------------------

size_t
ImmediateModelInstancesBatch::CurrentBatchSize() const
{
  return m_instancedInputBuffer.GetMaximumSize() / m_instancedDataSizeInBytes;
}

// ----------------------------------------------------------------------------

void
ImmediateModelInstancesBatch::ResizeBatch(const size_t batchSize)
{
  const size_t batchSizeInBytes = batchSize * m_instancedDataSizeInBytes;
  m_instancedInputBuffer.Resize(batchSizeInBytes);

  for (auto& buffer : m_gpuBuffers) {
    buffer.RecreateInstancedBuffer(
      RawArrayView{ nullptr, batchSize, m_instancedDataSizeInBytes },
      m_instancedAttributes);
  }
}

// ----------------------------------------------------------------------------
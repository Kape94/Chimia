#include "ImmediateModelInstancesBatch.h"

#include "BatchUtils.h"
#include "Core/Types.h"
#include "Rendering/InstancedBuffer.h"
#include "Rendering/ReusableIndexedVertexBufferObject.h"
#include "Rendering/ShaderAttribute.h"
#include "eImmediateFlushingPolicy.h"

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

  const size_t batchSizeInBytes = batchSize * m_instancedDataSizeInBytes;
  m_instancedInputBuffer.Resize(batchSizeInBytes);
  m_currentGPUBatchSizeInBytes = batchSizeInBytes;
}

// ----------------------------------------------------------------------------

void
ImmediateModelInstancesBatch::AddGPUBuffer(
  const Rendering::ReusableIndexedVertexBufferObject& bufferData,
  const size_t instanceBatchSize,
  const Rendering::ShaderAttributes& vertexAttributes,
  const Rendering::ShaderAttributes& instanceAttributes)
{
  Rendering::InstancedRenderAction& inserted = m_gpuActions.emplace_back();
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
ImmediateModelInstancesBatch::Flush(
  const eImmediateFlusingPolicy flushingPolicy)
{
  const size_t totalInputSize = m_instancedInputBuffer.GetSize();
  if (totalInputSize == 0) {
    return;
  }

  if (!BatchUtils::ShouldKeepInput(flushingPolicy)) {
    HandleDynamicResizing();
  }
  DoFlush(flushingPolicy);
}

// ----------------------------------------------------------------------------

void
ImmediateModelInstancesBatch::DoFlush(
  const eImmediateFlusingPolicy flushingPolicy)
{
  m_onFlush();

  BatchUtils::RenderInstancedByBatches(m_instancedInputBuffer.GetSize(),
                                       m_currentGPUBatchSizeInBytes,
                                       m_instancedDataSizeInBytes,
                                       m_instancedInputBuffer,
                                       m_gpuActions);

  if (!BatchUtils::ShouldKeepInput(flushingPolicy)) {
    m_instancedInputBuffer.Reset();
  }
}

// ----------------------------------------------------------------------------

void
ImmediateModelInstancesBatch::HandleDynamicResizing()
{
  const size_t maximumAllowed = m_batchingSettings.maximumBatchSize;
  const size_t gpuBatchSize = CurrentGPUBatchSize();

  const size_t inputSizeInBytes = m_instancedInputBuffer.GetSize();
  const size_t inputAmmount = inputSizeInBytes / m_instancedDataSizeInBytes;

  if (gpuBatchSize < maximumAllowed && gpuBatchSize < inputAmmount) {
    const size_t desiredBatchSize = inputAmmount;
    const size_t newBatchSize = std::min(desiredBatchSize, maximumAllowed);

    ResizeBatch(newBatchSize);
  }
}

// ----------------------------------------------------------------------------

size_t
ImmediateModelInstancesBatch::CurrentGPUBatchSize() const
{
  return m_currentGPUBatchSizeInBytes / m_instancedDataSizeInBytes;
}

// ----------------------------------------------------------------------------

void
ImmediateModelInstancesBatch::ResizeBatch(const size_t batchSize)
{
  const size_t batchSizeInBytes = batchSize * m_instancedDataSizeInBytes;

  for (auto& buffer : m_gpuActions) {
    buffer.RecreateInstancedBuffer(
      RawArrayView{ nullptr, batchSize, m_instancedDataSizeInBytes },
      m_instancedAttributes);
  }

  m_currentGPUBatchSizeInBytes = batchSizeInBytes;
}

// ----------------------------------------------------------------------------
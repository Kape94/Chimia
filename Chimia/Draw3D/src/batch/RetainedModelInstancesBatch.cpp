#include "RetainedModelInstancesBatch.h"

#include "BatchUtils.h"
#include "Core/DataBuffer.h"
#include "Core/Types.h"
#include "Rendering/InstancedBuffer.h"
#include "Rendering/ReusableIndexedVertexBufferObject.h"
#include "Rendering/ShaderAttribute.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

void
RetainedModelInstancesBatch::Create(
  const Model& model,
  const BatchingSettings& batchingSettings,
  const Rendering::ShaderAttributes& vertexAttributes,
  const Rendering::ShaderAttributes& instanceAttributes,
  const std::function<void()>& onRender)
{
  m_onRender = onRender;
  m_batchingSettings = batchingSettings;
  m_instancedAttributes = instanceAttributes;

  const size_t batchSize = batchingSettings.initialBatchSize;
  const size_t instanceDataSizeInBytes =
    instanceAttributes.ComputeTotalSizeOfAttributes();

  CreateGPUBuffers(model,
                   batchSize,
                   instanceDataSizeInBytes,
                   vertexAttributes,
                   instanceAttributes);

  m_instanceDataSizeInBytes = instanceDataSizeInBytes;
  m_currentGPUBatchSize = batchSize;

  const size_t batchDataSizeInBytes = batchSize * instanceDataSizeInBytes;
  m_instanceDataBuffer.Resize(batchDataSizeInBytes);
}

// ----------------------------------------------------------------------------

void
RetainedModelInstancesBatch::CreateGPUBuffers(
  const Model& model,
  const size_t batchSize,
  const size_t instanceBatchDataSize,
  const Rendering::ShaderAttributes& vertexAttributes,
  const Rendering::ShaderAttributes& instanceAttributes)
{
  model.ForEachBuffer(
    [&](const Rendering::ReusableIndexedVertexBufferObject& buffer) {
      Rendering::InstancedBuffer& gpuBuffer = m_gpuBuffers.emplace_back();

      gpuBuffer.CreateInstanced(
        buffer,
        vertexAttributes,
        RawArrayView{ nullptr, batchSize, instanceBatchDataSize },
        instanceAttributes);
    });
}

// ----------------------------------------------------------------------------

unsigned
RetainedModelInstancesBatch::AddInstance(const RawDataView& instanceData)
{
  auto [instanceID, newInstance] = m_instanceTable.Insert();
  newInstance->Append(instanceData);

  m_shouldRebuildBuffers = true;

  return instanceID;
}

// ----------------------------------------------------------------------------

unsigned
RetainedModelInstancesBatch::AddInstance(
  const std::initializer_list<RawDataView>& instanceDatas)
{
  auto [instanceID, newInstance] = m_instanceTable.Insert();
  for (const RawDataView& data : instanceDatas) {
    newInstance->Append(data);
  }

  m_shouldRebuildBuffers = true;

  return instanceID;
}

// ----------------------------------------------------------------------------

void
RetainedModelInstancesBatch::DeleteInstance(unsigned instanceID)
{
  m_instanceTable.Delete(instanceID);
  m_shouldRebuildBuffers = true;
}

// ----------------------------------------------------------------------------

void
RetainedModelInstancesBatch::Render()
{
  if (CanRenderWithCurrentBuffers()) {
    if (HasSomethingToRender()) {
      m_onRender();
      RenderCurrentBuffers();
    }
    return;
  }

  if (m_shouldRebuildBuffers) {
    RebuildInputBuffer();
    HandleDynamicResizing();
  }

  RenderByBatches();
}

// ----------------------------------------------------------------------------

bool
RetainedModelInstancesBatch::CanRenderWithCurrentBuffers() const
{
  const bool needToReloadDataOnGPU = m_shouldRebuildBuffers;

  const bool needToRenderByBatch =
    m_instanceDataBuffer.GetSize() > CurrentGPUBatchSizeInBytes();

  return !needToReloadDataOnGPU && !needToRenderByBatch;
}

// ----------------------------------------------------------------------------

bool
RetainedModelInstancesBatch::HasSomethingToRender() const
{
  return m_instanceDataBuffer.GetSize() > 0;
}

// ----------------------------------------------------------------------------

void
RetainedModelInstancesBatch::RebuildInputBuffer()
{
  m_instanceDataBuffer.Reset();
  m_instanceTable.ForEach([&](const DataBuffer& instanceData) {
    m_instanceDataBuffer.Append(instanceData.GetData(), instanceData.GetSize());
  });
  m_shouldRebuildBuffers = false;
}

// ----------------------------------------------------------------------------

void
RetainedModelInstancesBatch::HandleDynamicResizing()
{
  const size_t maximumAllowed = m_batchingSettings.maximumBatchSize;
  const size_t currentBatchSize = m_currentGPUBatchSize;

  const size_t nInstancesInBuffer =
    m_instanceDataBuffer.GetSize() / m_instanceDataSizeInBytes;
  if (currentBatchSize < maximumAllowed &&
      currentBatchSize != nInstancesInBuffer) {
    const size_t desiredBatchSize = nInstancesInBuffer;
    const size_t newBatchSize = std::min(desiredBatchSize, maximumAllowed);

    ResizeBatch(newBatchSize);
  }
}

// ----------------------------------------------------------------------------

void
RetainedModelInstancesBatch::ResizeBatch(const size_t batchSize)
{
  for (auto& buffer : m_gpuBuffers) {
    buffer.RecreateInstancedBuffer(
      RawArrayView{ nullptr, batchSize, m_instanceDataSizeInBytes },
      m_instancedAttributes);
  }
  m_currentGPUBatchSize = batchSize;
}

// ----------------------------------------------------------------------------

void
RetainedModelInstancesBatch::RenderByBatches()
{
  if (!HasSomethingToRender()) {
    return;
  }

  m_onRender();

  BatchUtils::RenderInstancedByBatches(m_instanceDataBuffer.GetSize(),
                                       CurrentGPUBatchSizeInBytes(),
                                       m_instanceDataSizeInBytes,
                                       m_instanceDataBuffer,
                                       m_gpuBuffers);
}

// ----------------------------------------------------------------------------

void
RetainedModelInstancesBatch::RenderCurrentBuffers()
{
  for (Rendering::InstancedBuffer& gpuBuffer : m_gpuBuffers) {
    gpuBuffer.Render();
  }
}

// ----------------------------------------------------------------------------

size_t
RetainedModelInstancesBatch::CurrentGPUBatchSizeInBytes() const
{
  return m_currentGPUBatchSize * m_instanceDataSizeInBytes;
}

// ----------------------------------------------------------------------------

#include "RetainedModelInstancesBatch.h"

#include "BatchUtils.h"
#include "Core/DataBuffer.h"
#include "Core/Types.h"
#include "Rendering/IndexData.h"
#include "Rendering/InstancedData.h"
#include "Rendering/VertexData.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

void
RetainedModelInstancesBatch::Create(
  const Model& model,
  const BatchingSettings& batchingSettings,
  const Rendering::DataLayout& instancedDataLayout,
  const ShaderBindingsTemplate& vertexBindingsTemplate,
  const ShaderBindingsTemplate& instancedBindingsTemplate,
  const std::function<void()>& onRender)
{
  m_onRender = onRender;
  m_batchingSettings = batchingSettings;
  m_instancedBindingsTemplate = instancedBindingsTemplate;

  const size_t batchSize = batchingSettings.initialBatchSize;
  const size_t instanceDataSizeInBytes = instancedDataLayout.TotalSize();

  CreateGPUBuffers(model,
                   batchSize,
                   instanceDataSizeInBytes,
                   instancedDataLayout,
                   vertexBindingsTemplate,
                   instancedBindingsTemplate);

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
  const Rendering::DataLayout& instancedDataLayout,
  const ShaderBindingsTemplate& vertexBindingsTemplate,
  const ShaderBindingsTemplate& instancedBindingsTemplate)
{
  model.ForEachBuffer([&](const Rendering::VertexDataInstance& vertexData,
                          const Rendering::IndexDataInstance& indexData) {
    BatchUtils::InstancedGPUComponent& gpuComponent =
      m_gpuComponent.emplace_back();

    gpuComponent.data = Rendering::InstancedData::New();
    gpuComponent.data->Create(
      RawDataView{ nullptr, batchSize * instanceBatchDataSize },
      instancedDataLayout);

    auto bindings = vertexBindingsTemplate.GenerateFor(vertexData);
    const auto instancedBindings =
      instancedBindingsTemplate.GenerateFor(gpuComponent.data);

    bindings.insert(
      bindings.end(), instancedBindings.begin(), instancedBindings.end());

    gpuComponent.action.Create(
      vertexBindingsTemplate.GetTarget(), indexData, bindings);
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
  const size_t effectiveBatchSize = BatchUtils::EffectiveBatchSize(batchSize);

  for (auto& component : m_gpuComponent) {
    component.data->Resize(
      RawDataView{ nullptr, effectiveBatchSize * m_instanceDataSizeInBytes });
  }
  m_currentGPUBatchSize = effectiveBatchSize;
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
                                       m_gpuComponent);
}

// ----------------------------------------------------------------------------

void
RetainedModelInstancesBatch::RenderCurrentBuffers()
{
  for (BatchUtils::InstancedGPUComponent& component : m_gpuComponent) {
    component.action.Render();
  }
}

// ----------------------------------------------------------------------------

size_t
RetainedModelInstancesBatch::CurrentGPUBatchSizeInBytes() const
{
  return m_currentGPUBatchSize * m_instanceDataSizeInBytes;
}

// ----------------------------------------------------------------------------

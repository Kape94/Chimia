#include "ImmediateModelInstancesBatch.h"

#include "BatchUtils.h"
#include "Core/Types.h"
#include "Rendering/IndexData.h"
#include "Rendering/InstancedData.h"
#include "Rendering/RenderAction.h"
#include "Rendering/VertexData.h"
#include "eImmediateFlushingPolicy.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

void
ImmediateModelInstancesBatch::Create(
  const Model& model,
  const BatchingSettings& batchingSettings,
  const Rendering::DataLayout& instancedDataLayout,
  const ShaderBindingsTemplate& vertexBindingsTemplate,
  const ShaderBindingsTemplate& instancedBindingsTemplate,
  const std::function<void(void)>& onFlush)
{
  m_onFlush = onFlush;
  m_batchingSettings = batchingSettings;
  m_instancedBindingsTemplates = instancedBindingsTemplate;
  m_instancedDataSizeInBytes = instancedDataLayout.TotalSize();

  const size_t batchSize = batchingSettings.initialBatchSize;
  model.ForEachBuffer([&](const Rendering::VertexDataInstance& vertexData,
                          const Rendering::IndexDataInstance& indexData) {
    AddGPUBuffer(vertexData,
                 indexData,
                 batchSize,
                 instancedDataLayout,
                 vertexBindingsTemplate,
                 instancedBindingsTemplate);
  });

  const size_t batchSizeInBytes = batchSize * m_instancedDataSizeInBytes;
  m_instancedInputBuffer.Resize(batchSizeInBytes);
  m_currentGPUBatchSizeInBytes = batchSizeInBytes;
}

// ----------------------------------------------------------------------------

void
ImmediateModelInstancesBatch::AddGPUBuffer(
  const Rendering::VertexDataInstance& vertexData,
  const Rendering::IndexDataInstance& indexData,
  const size_t instanceBatchSize,
  const Rendering::DataLayout& instancedDataLayout,
  const ShaderBindingsTemplate& vertexBindingsTemplate,
  const ShaderBindingsTemplate& instancedBindingsTemplate)
{
  BatchUtils::InstancedGPUComponent& inserted = m_gpuComponents.emplace_back();

  inserted.data = Rendering::InstancedData::Create(
    RawDataView{ nullptr, instanceBatchSize * m_instancedDataSizeInBytes },
    instancedDataLayout);

  auto bindings = vertexBindingsTemplate.GenerateFor(vertexData);
  const auto instancedBindings =
    instancedBindingsTemplate.GenerateFor(inserted.data);

  bindings.insert(
    bindings.end(), instancedBindings.begin(), instancedBindings.end());
  inserted.action.Create(
    vertexBindingsTemplate.GetTarget(), indexData, bindings);
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
                                       m_gpuComponents);

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

  for (auto& gpuComponent : m_gpuComponents) {
    gpuComponent.data->Resize(
      RawDataView{ nullptr, batchSize * m_instancedDataSizeInBytes });
  }

  m_currentGPUBatchSizeInBytes = batchSizeInBytes;
}

// ----------------------------------------------------------------------------
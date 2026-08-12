#include "ImmediateTransitionsBatch.h"

#include "BatchUtils.h"
#include "Core/Types.h"
#include "Rendering/IndexData.h"
#include "Rendering/InstancedData.h"
#include "Rendering/RenderAction.h"
#include "Rendering/VertexData.h"
#include "ResourcesManager.h"
#include "eImmediateFlushingPolicy.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

void
ImmediateTransitionsBatch::Create(
  const Transition& transition,
  const BatchingSettings& batchingSettings,
  const Rendering::DataLayout& instancedDataLayout,
  const ShaderBindingsTemplate& startBindingsTemplate,
  const ShaderBindingsTemplate& targetBindingsTemplate,
  const ShaderBindingsTemplate& instancedBindingsTemplate,
  const std::function<void(void)>& onFlush)
{
  m_onFlush = onFlush;
  m_batchingSettings = batchingSettings;
  m_instancedBindingsTemplates = instancedBindingsTemplate;
  m_instancedDataSizeInBytes = instancedDataLayout.TotalSize();

  const size_t batchSize = batchingSettings.initialBatchSize;

  auto& resources = ResourcesManager::GetInstance();
  const Model* start = resources.GetModel(transition.GetStart());
  const Model* target = resources.GetModel(transition.GetTarget());

  std::vector<std::pair<const Rendering::VertexDataInstance,
                        const Rendering::IndexDataInstance>>
    startDatas;

  start->ForEachBuffer(
    [&startDatas](const Rendering::VertexDataInstance& vertex,
                  const Rendering::IndexDataInstance& index) {
      startDatas.emplace_back(std::make_pair(vertex, index));
    });

  std::vector<std::pair<const Rendering::VertexDataInstance,
                        const Rendering::IndexDataInstance>>
    targetDatas;

  target->ForEachBuffer(
    [&targetDatas](const Rendering::VertexDataInstance& vertex,
                   const Rendering::IndexDataInstance& index) {
      targetDatas.emplace_back(std::make_pair(vertex, index));
    });

  assert(startDatas.size() == targetDatas.size() &&
         "TransitionsBatch: start and target models are not compatible");

  for (size_t i = 0; i < startDatas.size(); ++i) {
    BatchUtils::InstancedGPUComponent& inserted =
      m_gpuComponents.emplace_back();

    inserted.data = Rendering::InstancedData::Create(
      RawDataView{ nullptr, batchSize * m_instancedDataSizeInBytes },
      instancedDataLayout);

    const auto& vertexStart = startDatas[i].first;
    const auto& vertexTarget = targetDatas[i].first;
    const auto& index = startDatas[i].second;

    auto bindings = startBindingsTemplate.GenerateFor(vertexStart);

    auto targetBindings = targetBindingsTemplate.GenerateFor(vertexTarget);
    auto instancedBindings =
      instancedBindingsTemplate.GenerateFor(inserted.data);

    bindings.insert(
      bindings.end(), targetBindings.begin(), targetBindings.end());
    bindings.insert(
      bindings.end(), instancedBindings.begin(), instancedBindings.end());

    inserted.action.Create(startBindingsTemplate.GetTarget(),
                           index,
                           bindings,
                           Rendering::ePrimitive::TRIANGLES);
  }

  const size_t batchSizeInBytes = batchSize * m_instancedDataSizeInBytes;
  m_instancedInputBuffer.Resize(batchSizeInBytes);
  m_currentGPUBatchSizeInBytes = batchSizeInBytes;
}

// ----------------------------------------------------------------------------

void
ImmediateTransitionsBatch::Draw(const RawDataView& instanceData)
{
  m_instancedInputBuffer.Append(instanceData);
}

// ----------------------------------------------------------------------------

void
ImmediateTransitionsBatch::Draw(
  const std::initializer_list<RawDataView>& instanceDatas)
{
  for (const auto& instanceData : instanceDatas) {
    Draw(instanceData);
  }
}

// ----------------------------------------------------------------------------

void
ImmediateTransitionsBatch::Flush(const eImmediateFlusingPolicy flushingPolicy)
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
ImmediateTransitionsBatch::DoFlush(const eImmediateFlusingPolicy flushingPolicy)
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
ImmediateTransitionsBatch::HandleDynamicResizing()
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
ImmediateTransitionsBatch::CurrentGPUBatchSize() const
{
  return m_currentGPUBatchSizeInBytes / m_instancedDataSizeInBytes;
}

// ----------------------------------------------------------------------------

void
ImmediateTransitionsBatch::ResizeBatch(const size_t batchSize)
{
  const size_t batchSizeInBytes = batchSize * m_instancedDataSizeInBytes;

  for (auto& gpuComponent : m_gpuComponents) {
    gpuComponent.data->Resize(
      RawDataView{ nullptr, batchSize * m_instancedDataSizeInBytes });
  }

  m_currentGPUBatchSizeInBytes = batchSizeInBytes;
}

// ----------------------------------------------------------------------------
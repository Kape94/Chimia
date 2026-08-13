#include "ImmediateTransitionsBatch.h"

#include "BatchUtils.h"
#include "Core/Types.h"
#include "DataBindingProvider.h"
#include "Rendering/DataLayout.h"
#include "Rendering/IndexData.h"
#include "Rendering/InstancedData.h"
#include "Rendering/RenderAction.h"
#include "Rendering/VertexData.h"
#include "ResourcesManager.h"
#include "eImmediateFlushingPolicy.h"
#include <vector>

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
// Utils
// ----------------------------------------------------------------------------

namespace {

std::vector<Chimia::Rendering::RenderAction::Binding>
GenerateBindings(const Chimia::Rendering::VertexDataInstance& vertexStart,
                 const Chimia::Rendering::VertexDataInstance& vertexTarget,
                 const Chimia::Rendering::InstancedDataInstance& instancedData,
                 const DataBindingProvider& dataBindings)
{
  auto bindings = dataBindings.GetVertexTemplate().GenerateFor(vertexStart);

  auto targetBindings =
    dataBindings.GetTargetVertexTemplate().GenerateFor(vertexTarget);
  auto instancedBindings =
    dataBindings.GetInstancedTransitionTemplate().GenerateFor(instancedData);

  bindings.insert(bindings.end(), targetBindings.begin(), targetBindings.end());
  bindings.insert(
    bindings.end(), instancedBindings.begin(), instancedBindings.end());

  return bindings;
}

}

// ----------------------------------------------------------------------------
// ImmediateTransitionsBatch
// ----------------------------------------------------------------------------

void
ImmediateTransitionsBatch::Create(const Transition& transition,
                                  const BatchingSettings& batchingSettings,
                                  const DataBindingProvider& dataBindings,
                                  const std::function<void(void)>& onFlush)
{
  m_onFlush = onFlush;
  m_batchingSettings = batchingSettings;
  m_instancedDataSizeInBytes =
    dataBindings.GetInstancedTransitionLayout().TotalSize();

  auto& resources = ResourcesManager::GetInstance();
  const Model* start = resources.GetModel(transition.GetStart());
  const Model* target = resources.GetModel(transition.GetTarget());

  const std::vector<ModelVertexAndIndex> startDatas =
    ExtractGPUDatasFromModel(*start);
  const std::vector<ModelVertexAndIndex> targetDatas =
    ExtractGPUDatasFromModel(*target);

  assert(startDatas.size() == targetDatas.size() &&
         "TransitionsBatch: start and target models are not compatible");

  const size_t batchSize = batchingSettings.initialBatchSize;
  CreateGPUActions(startDatas, targetDatas, dataBindings, batchSize);

  const size_t batchSizeInBytes = batchSize * m_instancedDataSizeInBytes;
  m_instancedInputBuffer.Resize(batchSizeInBytes);
  m_currentGPUBatchSizeInBytes = batchSizeInBytes;
}

// ----------------------------------------------------------------------------

std::vector<ImmediateTransitionsBatch::ModelVertexAndIndex>
ImmediateTransitionsBatch::ExtractGPUDatasFromModel(const Model& model) const
{
  std::vector<ModelVertexAndIndex> gpuDatas;

  model.ForEachBuffer(
    [&gpuDatas](const Chimia::Rendering::VertexDataInstance& vertex,
                const Chimia::Rendering::IndexDataInstance& index) {
      gpuDatas.emplace_back(std::make_pair(vertex, index));
    });

  return gpuDatas;
}

// ----------------------------------------------------------------------------

void
ImmediateTransitionsBatch::CreateGPUActions(
  const std::vector<ModelVertexAndIndex>& startDatas,
  const std::vector<ModelVertexAndIndex>& targetDatas,
  const DataBindingProvider& dataBindings,
  const size_t batchSize)
{
  for (size_t i = 0; i < startDatas.size(); ++i) {
    BatchUtils::InstancedGPUComponent& inserted =
      m_gpuComponents.emplace_back();

    inserted.data = Rendering::InstancedData::Create(
      RawDataView{ nullptr, batchSize * m_instancedDataSizeInBytes },
      dataBindings.GetInstancedTransitionLayout());

    const auto& vertexStart = startDatas[i].first;
    const auto& vertexTarget = targetDatas[i].first;
    const auto& index = startDatas[i].second;

    auto bindings =
      GenerateBindings(vertexStart, vertexTarget, inserted.data, dataBindings);
    inserted.action.Create(dataBindings.GetRenderingTarget(),
                           index,
                           bindings,
                           Rendering::ePrimitive::TRIANGLES);
  }
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
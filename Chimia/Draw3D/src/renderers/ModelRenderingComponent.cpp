#include "ModelRenderingComponent.h"

#include "Draw3DPrivate.h"
#include "ResourcesManager.h"

#include "Core/Diagnostics.h"
#include "Types.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

void
ModelRenderingComponent::Init(
  const BatchingSettings& batchingSettings,
  const Rendering::DataLayout& instancedDataLayout,
  const ShaderBindingsTemplate& vertexBindingsTemplates,
  const ShaderBindingsTemplate& instancedBindingsTemplates,
  const std::function<void(void)>& onFlush)
{
  m_batchingSettings = batchingSettings;
  m_instancedDataLayout = instancedDataLayout;
  m_vertexBindingsTemplates = vertexBindingsTemplates;
  m_instancedBindingsTemplates = instancedBindingsTemplates;
  m_onFlush = onFlush;
}

// ----------------------------------------------------------------------------

void
ModelRenderingComponent::Flush(const eImmediateFlusingPolicy flushingPolicy)
{
  m_transformedModelsTable.ForEach(
    [flushingPolicy](ImmediateModelInstancesBatch& model) {
      model.Flush(flushingPolicy);
    });
  m_staticModelsTable.ForEach(
    [](RetainedModelInstancesBatch& model) { model.Render(); });
}

// ----------------------------------------------------------------------------

void
ModelRenderingComponent::DrawModel(const ModelID& modelID,
                                   const RawDataView& instanceData)
{
  const unsigned id = Draw3DPrivate::GetModelID(modelID);
  ImmediateModelInstancesBatch* batch = m_transformedModelsTable.Find(id);
  if (batch == nullptr) {
    batch = AllocateImmediateBatchForModelInstances(modelID);
    if (batch == nullptr) {
      return;
    }
  }

  batch->Draw(instanceData);
}

// ----------------------------------------------------------------------------

void
ModelRenderingComponent::DrawModel(
  const ModelID& modelID,
  const std::initializer_list<RawDataView>& instanceDatas)
{
  const unsigned id = Draw3DPrivate::GetModelID(modelID);
  ImmediateModelInstancesBatch* batch = m_transformedModelsTable.Find(id);
  if (batch == nullptr) {
    batch = AllocateImmediateBatchForModelInstances(modelID);
    if (batch == nullptr) {
      return;
    }
  }

  batch->Draw(instanceDatas);
}

// ----------------------------------------------------------------------------

ImmediateModelInstancesBatch*
ModelRenderingComponent::AllocateImmediateBatchForModelInstances(
  const ModelID& modelID)
{
  const unsigned id = Draw3DPrivate::GetModelID(modelID);

  ImmediateModelInstancesBatch* batch =
    m_transformedModelsTable.InsertWithID(id);
  if (batch == nullptr) {
    Chimia::Diagnostics::Error(
      1,
      "Unexpected error at ModelRenderingComponent::DrawModel, couldn't "
      "create transformed model entry for new model created");
    return nullptr;
  }

  const Model* model = ResourcesManager::GetInstance().GetModel(modelID);
  batch->Create(*model,
                m_batchingSettings,
                m_instancedDataLayout,
                m_vertexBindingsTemplates,
                m_instancedBindingsTemplates,
                m_onFlush);
  return batch;
}

// ----------------------------------------------------------------------------

LocalModelInstanceID
ModelRenderingComponent::AddRetainedModel(const ModelID& modelID,
                                          const RawDataView& instanceData)
{
  const unsigned id = Draw3DPrivate::GetModelID(modelID);

  RetainedModelInstancesBatch* staticModel = m_staticModelsTable.Find(id);
  if (staticModel == nullptr) {
    staticModel = AllocateRetainedBatchForModelInstances(modelID);
    if (staticModel == nullptr) {
      return Draw3DPrivate::CreateLocalModelInstanceID(0, 0);
    }
  }

  const unsigned instanceID = staticModel->AddInstance(instanceData);
  return Draw3DPrivate::CreateLocalModelInstanceID(id, instanceID);
}

// ----------------------------------------------------------------------------

LocalModelInstanceID
ModelRenderingComponent::AddRetainedModel(
  const ModelID& modelID,
  const std::initializer_list<RawDataView>& instanceDatas)
{
  const unsigned id = Draw3DPrivate::GetModelID(modelID);

  RetainedModelInstancesBatch* staticModel = m_staticModelsTable.Find(id);
  if (staticModel == nullptr) {
    staticModel = AllocateRetainedBatchForModelInstances(modelID);
    if (staticModel == nullptr) {
      return Draw3DPrivate::CreateLocalModelInstanceID(0, 0);
    }
  }

  const unsigned instanceID = staticModel->AddInstance(instanceDatas);
  return Draw3DPrivate::CreateLocalModelInstanceID(id, instanceID);
}

// ----------------------------------------------------------------------------

RetainedModelInstancesBatch*
ModelRenderingComponent::AllocateRetainedBatchForModelInstances(
  const ModelID& modelID)
{
  const unsigned id = Draw3DPrivate::GetModelID(modelID);

  RetainedModelInstancesBatch* staticModel =
    m_staticModelsTable.InsertWithID(id);
  if (staticModel == nullptr) {
    Chimia::Diagnostics::Error(
      1,
      "Unexpected error at ModelRenderingComponent::AddStaticModel, couldn't "
      "create static model entry for new model created");
    return nullptr;
  }

  const Model* model = ResourcesManager::GetInstance().GetModel(modelID);
  staticModel->Create(*model,
                      m_batchingSettings,
                      m_instancedDataLayout,
                      m_vertexBindingsTemplates,
                      m_instancedBindingsTemplates,
                      m_onFlush);
  return staticModel;
}

// ----------------------------------------------------------------------------

void
ModelRenderingComponent::DeleteRetainedModel(
  const LocalModelInstanceID& instanceID)
{
  const auto [modelIDValue, instanceIDValue] =
    Draw3DPrivate::GetLocalModelInstanceIDValues(instanceID);

  RetainedModelInstancesBatch* model = m_staticModelsTable.Find(modelIDValue);
  if (model == nullptr) {
    return;
  }

  model->DeleteInstance(instanceIDValue);
}

// ----------------------------------------------------------------------------
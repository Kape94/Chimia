#include "ModelRenderingComponent.h"

#include "Draw3DPrivate.h"
#include "ModelBatch.h"
#include "ResourcesManager.h"

#include "Core/Diagnostics.h"
#include "Rendering/ShaderAttribute.h"
#include "StaticModel.h"
#include "Types.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

void
ModelRenderingComponent::Init(
  const size_t modelBatchSize,
  const Rendering::ShaderAttributes& vertexAttributes,
  const Rendering::ShaderAttributes& instanceAttributes,
  const std::function<void(void)>& onFlush)
{
  m_batchSize = modelBatchSize;
  m_vertexAttributes = vertexAttributes;
  m_instanceAttributes = instanceAttributes;
  m_onFlush = onFlush;
}

// ----------------------------------------------------------------------------

void
ModelRenderingComponent::Flush()
{
  m_transformedModelsTable.ForEach([](ModelBatch& model) { model.Flush(); });
  m_staticModelsTable.ForEach([](StaticModel& model) { model.Render(); });
}

// ----------------------------------------------------------------------------

void
ModelRenderingComponent::DrawModel(const ModelID& modelID,
                                   const RawDataView& instanceData)
{
  const unsigned id = Draw3DPrivate::GetModelID(modelID);
  ModelBatch* batch = m_transformedModelsTable.Find(id);
  if (batch == nullptr) {
    batch = AllocateBatchForModelDrawing(modelID);
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
  ModelBatch* batch = m_transformedModelsTable.Find(id);
  if (batch == nullptr) {
    batch = AllocateBatchForModelDrawing(modelID);
    if (batch == nullptr) {
      return;
    }
  }

  batch->Draw(instanceDatas);
}

// ----------------------------------------------------------------------------

ModelBatch*
ModelRenderingComponent::AllocateBatchForModelDrawing(const ModelID& modelID)
{
  const unsigned id = Draw3DPrivate::GetModelID(modelID);

  ModelBatch* batch = m_transformedModelsTable.Insert(id);
  if (batch == nullptr) {
    Chimia::Diagnostics::Error(
      1,
      "Unexpected error at ModelRenderingComponent::DrawModel, couldn't "
      "create transformed model entry for new model created");
    return nullptr;
  }

  const Model* model = ResourcesManager::GetInstance().GetModel(modelID);
  batch->Create(
    *model, m_batchSize, m_vertexAttributes, m_instanceAttributes, m_onFlush);
  return batch;
}

// ----------------------------------------------------------------------------

ModelInstanceID
ModelRenderingComponent::AddStaticModel(const ModelID& modelID,
                                        const RawDataView& instanceData)
{
  const unsigned id = Draw3DPrivate::GetModelID(modelID);

  StaticModel* staticModel = m_staticModelsTable.Find(id);
  if (staticModel == nullptr) {
    staticModel = AllocateBatchForStaticModel(modelID);
    if (staticModel == nullptr) {
      return Draw3DPrivate::CreateModelInstanceID(0, 0);
    }
  }

  const unsigned instanceID = staticModel->AddInstance(instanceData);
  return Draw3DPrivate::CreateModelInstanceID(id, instanceID);
}

// ----------------------------------------------------------------------------

ModelInstanceID
ModelRenderingComponent::AddStaticModel(
  const ModelID& modelID,
  const std::initializer_list<RawDataView>& instanceDatas)
{
  const unsigned id = Draw3DPrivate::GetModelID(modelID);

  StaticModel* staticModel = m_staticModelsTable.Find(id);
  if (staticModel == nullptr) {
    staticModel = AllocateBatchForStaticModel(modelID);
    if (staticModel == nullptr) {
      return Draw3DPrivate::CreateModelInstanceID(0, 0);
    }
  }

  const unsigned instanceID = staticModel->AddInstance(instanceDatas);
  return Draw3DPrivate::CreateModelInstanceID(id, instanceID);
}

// ----------------------------------------------------------------------------

StaticModel*
ModelRenderingComponent::AllocateBatchForStaticModel(const ModelID& modelID)
{
  const unsigned id = Draw3DPrivate::GetModelID(modelID);

  StaticModel* staticModel = m_staticModelsTable.Insert(id);
  if (staticModel == nullptr) {
    Chimia::Diagnostics::Error(
      1,
      "Unexpected error at ModelRenderingComponent::AddStaticModel, couldn't "
      "create static model entry for new model created");
    return nullptr;
  }

  const Model* model = ResourcesManager::GetInstance().GetModel(modelID);
  staticModel->Create(
    *model, m_batchSize, m_vertexAttributes, m_instanceAttributes, m_onFlush);
  return staticModel;
}

// ----------------------------------------------------------------------------

void
ModelRenderingComponent::DeleteStaticModel(const ModelInstanceID& instanceID)
{
  const auto [modelIDValue, instanceIDValue] =
    Draw3DPrivate::GetModelInstanceIDs(instanceID);

  StaticModel* model = m_staticModelsTable.Find(modelIDValue);
  if (model == nullptr) {
    return;
  }

  model->DeleteInstance(instanceIDValue);
}

// ----------------------------------------------------------------------------
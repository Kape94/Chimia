#include "ModelRenderingComponent.h"

#include "Draw3DPrivate.h"

#include "Core/Diagnostics.h"
#include "Rendering/ShaderAttribute.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

void
ModelRenderingComponent::Flush()
{
  m_transformedModelsTable.ForEach([](ModelBatch& model) { model.Flush(); });
  m_staticModelsTable.ForEach([](StaticModel& model) { model.Render(); });
}

// ----------------------------------------------------------------------------

ModelID
ModelRenderingComponent::CreateModel(
  const std::vector<float>& vertexData,
  const std::vector<unsigned>& indices,
  const size_t modelBatchSize,
  const Rendering::ShaderAttributes& vertexAttributes,
  const Rendering::ShaderAttributes& instanceAttributes,
  const std::function<void(void)>& onFlush)
{
  // TODO: handle unnecessary vector copy (BufferData holds vector member that
  // copy the parameters)
  const size_t sizePerVertex = vertexAttributes.ComputeTotalSizeOfAttributes();
  const size_t nVertices = (vertexData.size() * sizeof(float)) / sizePerVertex;

  auto [modelID, model] = m_modelsTable.Insert();
  model->Create(MeshDataView(vertexData, nVertices, indices));

  ModelBatch* batch = m_transformedModelsTable.Insert(modelID);
  if (batch == nullptr) {
    Chimia::Diagnostics::Error(
      1,
      "Unexpected error at VertexColoredRendererImpl::CreateModel, couldn't "
      "create transformed model entry for new model created");
  }

  batch->Create(
    *model, modelBatchSize, vertexAttributes, instanceAttributes, onFlush);

  StaticModel* staticModel = m_staticModelsTable.Insert(modelID);
  if (staticModel == nullptr) {
    Chimia::Diagnostics::Error(
      1,
      "Unexpected error at VertexColoredRendererImpl::CreateModel, couldn't "
      "create static model entry for new model created");
  }

  staticModel->Create(
    *model, modelBatchSize, vertexAttributes, instanceAttributes, onFlush);

  return Draw3DPrivate::CreateModelID(modelID);
}

// ----------------------------------------------------------------------------

void
ModelRenderingComponent::DrawModel(const ModelID& modelID,
                                   const RawDataView& instanceData)
{
  const unsigned id = Draw3DPrivate::GetModelID(modelID);
  ModelBatch* batch = m_transformedModelsTable.Find(id);
  if (batch == nullptr) {
    return;
  }

  batch->Draw(instanceData);
}

// ----------------------------------------------------------------------------

ModelInstanceID
ModelRenderingComponent::AddStaticModel(const ModelID& modelID,
                                        const RawDataView& instanceData)
{
  const unsigned id = Draw3DPrivate::GetModelID(modelID);
  StaticModel* model = m_staticModelsTable.Find(id);
  if (model == nullptr) {
    return Draw3DPrivate::CreateModelInstanceID(0, 0);
  }

  const unsigned instanceID = model->AddInstance(instanceData);

  return Draw3DPrivate::CreateModelInstanceID(id, instanceID);
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
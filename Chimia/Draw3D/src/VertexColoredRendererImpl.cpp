#include "VertexColoredRendererImpl.h"

#include "BufferData.h"
#include "CameraPrivate.h"
#include "Config.h"
#include "Diagnostics/Diagnostics.h"
#include "Draw3DPrivate.h"
#include "ModelBatch.h"
#include "Shaders.h"

#include "Rendering/Shader.h"
#include "Rendering/ShaderAttribute.h"
#include "StaticModel.h"
#include "StaticTriangles.h"
#include "Types.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace {

static const Chimia::Rendering::ShaderAttributes VERTEX_ATTRIBUTES{
  Chimia::Rendering::ShaderAttribute::Float(0 /*pos*/, 3),
  Chimia::Rendering::ShaderAttribute::Float(1 /*color*/, 3)
};

static const Chimia::Rendering::ShaderAttributes
  TRANSFORMED_MODELS_INSTANCE_ATTRIBUTES{
    Chimia::Rendering::ShaderAttribute::Float(2 /*transform*/, 4),
    Chimia::Rendering::ShaderAttribute::Float(3 /*transform*/, 4),
    Chimia::Rendering::ShaderAttribute::Float(4 /*transform*/, 4),
    Chimia::Rendering::ShaderAttribute::Float(5 /*transform*/, 4)
  };

}

// ----------------------------------------------------------------------------

VertexColoredRendererImpl&
VertexColoredRendererImpl::getInstance()
{
  static VertexColoredRendererImpl renderer;
  return renderer;
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::Init()
{
  m_triangleBatch.Create(Config::VertexColored::triangleBatchSize,
                         VERTEX_ATTRIBUTES,
                         [&]() { ConfigureShaderForTriangleDrawing(); });

  m_indexedTriangleBatch.Create(
    Config::VertexColored::indexedTrianglesVertexBatchSize,
    Config::VertexColored::indexedTrianglesIndexBatchSize,
    VERTEX_ATTRIBUTES,
    [&]() { ConfigureShaderForTriangleDrawing(); });
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::DrawTriangle(const glm::vec3& p1,
                                        const glm::vec3& color1,
                                        const glm::vec3& p2,
                                        const glm::vec3& color2,
                                        const glm::vec3& p3,
                                        const glm::vec3& color3)
{
  constexpr size_t VEC3_SIZE = sizeof(glm::vec3);

  m_triangleBatch.Draw({
    { &p1, VEC3_SIZE },
    { &color1, VEC3_SIZE },
    { &p2, VEC3_SIZE },
    { &color2, VEC3_SIZE },
    { &p3, VEC3_SIZE },
    { &color3, VEC3_SIZE },
  });
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::DrawTriangles(const std::vector<float>& vertexData)
{
  m_triangleBatch.Draw({ vertexData.data(), vertexData.size(), sizeof(float) });
}

// ----------------------------------------------------------------------------

unsigned
VertexColoredRendererImpl::AddStaticTriangles(
  const std::vector<float>& vertexData)
{
  auto [id, triangles] = m_staticTriangles.Insert();
  triangles->Create(vertexData, VERTEX_ATTRIBUTES);

  return id;
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::DeleteStaticTriangles(unsigned id)
{
  m_staticTriangles.Delete(id);
}

// ----------------------------------------------------------------------------

ModelID
VertexColoredRendererImpl::CreateModel(const std::vector<float>& vertexData,
                                       const std::vector<unsigned>& indices)
{
  auto [modelID, model] = m_modelsTable.Insert();
  model->Create(BufferData(vertexData, indices));

  ModelBatch* batch = m_transformedModelsTable.Insert(modelID);
  if (batch == nullptr) {
    Chimia::Diagnostics::Error(
      1,
      "Unexpected error at VertexColoredRendererImpl::CreateModel, couldn't "
      "create transformed model entry for new model created");
  }

  auto configureShaderFn = [&]() {
    ConfigureShaderForTransformedModelDrawing();
  };

  const size_t instanceBathSize = Config::VertexColored::modelsBatchSize;
  batch->Create(*model,
                instanceBathSize,
                VERTEX_ATTRIBUTES,
                TRANSFORMED_MODELS_INSTANCE_ATTRIBUTES,
                configureShaderFn);

  StaticModel* staticModel = m_staticModelsTable.Insert(modelID);
  if (staticModel == nullptr) {
    Chimia::Diagnostics::Error(
      1,
      "Unexpected error at VertexColoredRendererImpl::CreateModel, couldn't "
      "create static model entry for new model created");
  }

  staticModel->Create(*model,
                      instanceBathSize,
                      VERTEX_ATTRIBUTES,
                      TRANSFORMED_MODELS_INSTANCE_ATTRIBUTES,
                      configureShaderFn);

  return Draw3DPrivate::CreateModelID(modelID);
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::DrawModelTransformed(const ModelID& modelID,
                                                const glm::mat4x4& transform)
{
  const unsigned id = Draw3DPrivate::GetModelID(modelID);
  ModelBatch* batch = m_transformedModelsTable.Find(id);
  if (batch == nullptr) {
    return;
  }

  batch->Draw({ &transform, sizeof(glm::mat4x4) });
}

// ----------------------------------------------------------------------------

ModelInstanceID
VertexColoredRendererImpl::AddStaticModel(const ModelID& modelID,
                                          const glm::mat4x4& transform)
{
  const unsigned id = Draw3DPrivate::GetModelID(modelID);
  StaticModel* model = m_staticModelsTable.Find(id);
  if (model == nullptr) {
    return Draw3DPrivate::CreateModelInstanceID(0, 0);
  }

  const unsigned instanceID =
    model->AddInstance({ &transform, sizeof(glm::mat4x4) });

  return Draw3DPrivate::CreateModelInstanceID(id, instanceID);
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::DeleteStaticModel(const ModelInstanceID& instanceID)
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

void
VertexColoredRendererImpl::DrawIndexedTriangles(
  const std::vector<float>& vertexData,
  const std::vector<unsigned>& indexData)
{
  m_indexedTriangleBatch.Draw(
    { vertexData.data(), vertexData.size(), sizeof(float) }, indexData);
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::Flush()
{
  m_triangleBatch.Flush();
  m_indexedTriangleBatch.Flush();

  m_transformedModelsTable.ForEach([](ModelBatch& model) { model.Flush(); });
  m_staticModelsTable.ForEach([](StaticModel& model) { model.Render(); });

  ConfigureShaderForTriangleDrawing();
  m_staticTriangles.ForEach(
    [](const StaticTriangles& triangles) { triangles.Render(); });
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::ConfigureShaderForTriangleDrawing()
{
  Rendering::Shader& shader = Shaders::VertexColored();
  shader.Use();
  shader.SetUniform("transform", CameraPrivate::GetCurrentTransform());
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::ConfigureShaderForTransformedModelDrawing()
{
  Rendering::Shader& shader = Shaders::VertexColoredWithInstancedTransform();
  shader.Use();
  shader.SetUniform("transform", CameraPrivate::GetCurrentTransform());
}

// ----------------------------------------------------------------------------

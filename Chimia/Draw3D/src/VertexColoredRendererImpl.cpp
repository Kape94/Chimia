#include "VertexColoredRendererImpl.h"

#include "CameraPrivate.h"
#include "Config.h"
#include "ModelBatch.h"
#include "Shaders.h"

#include "Rendering/Shader.h"
#include "Rendering/ShaderAttribute.h"
#include "StaticTriangles.h"

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
  auto newEntry = m_staticTriangles.Insert();
  StaticTriangles& triangles = *newEntry.second;

  triangles.Create(vertexData, VERTEX_ATTRIBUTES);

  return newEntry.first;
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::DeleteStaticTriangles(unsigned id)
{
  m_staticTriangles.Delete(id);
}

// ----------------------------------------------------------------------------

unsigned
VertexColoredRendererImpl::CreateModel(const std::vector<float>& vertexData,
                                       const std::vector<unsigned>& indices)
{
  std::pair<unsigned, ModelBatch*> inserted = m_transformedModelsTable.Insert();
  const unsigned modelID = inserted.first;

  const size_t instanceBathSize = Config::VertexColored::modelsBatchSize;
  ModelBatch& model = *inserted.second;
  model.Create({ vertexData, indices },
               instanceBathSize,
               VERTEX_ATTRIBUTES,
               TRANSFORMED_MODELS_INSTANCE_ATTRIBUTES,
               [&]() { ConfigureShaderForTransformedModelDrawing(); });

  return modelID;
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::DrawModelTransformed(unsigned modelID,
                                                const glm::mat4x4& transform)
{
  ModelBatch* model = m_transformedModelsTable.Find(modelID);
  if (model == nullptr) {
    return;
  }

  model->Draw({ &transform, sizeof(glm::mat4x4) });
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

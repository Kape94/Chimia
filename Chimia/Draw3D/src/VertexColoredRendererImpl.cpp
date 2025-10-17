#include "VertexColoredRendererImpl.h"

#include "CameraPrivate.h"
#include "Config.h"
#include "ModelBatch.h"
#include "Shaders.h"

#include "Rendering/Shader.h"
#include "Rendering/ShaderAttribute.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

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
  const size_t batchSize = Config::VertexColored::triangleBatchSize;

  m_triangleBatch.Create(batchSize,
                         { Rendering::ShaderAttribute::Float(0, 3),
                           Rendering::ShaderAttribute::Float(1, 3) },
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
               { Rendering::ShaderAttribute::Float(0, 3),
                 Rendering::ShaderAttribute::Float(1, 3) },
               { Rendering::ShaderAttribute::Float(2, 4),
                 Rendering::ShaderAttribute::Float(3, 4),
                 Rendering::ShaderAttribute::Float(4, 4),
                 Rendering::ShaderAttribute::Float(5, 4) },
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
VertexColoredRendererImpl::Flush()
{
  FlushTriangles();
  FlushTransformedModels();
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::FlushTriangles()
{
  m_triangleBatch.Flush();
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::FlushTransformedModels()
{
  m_transformedModelsTable.ForEach([](ModelBatch& model) { model.Flush(); });
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

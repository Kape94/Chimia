#include "VertexColoredRendererImpl.h"

#include "CameraPrivate.h"
#include "Rendering/ShaderAttribute.h"
#include "Shaders.h"

#include "Rendering/Shader.h"

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
  // TODO: batch size should be configurable
  constexpr size_t BATCH_SIZE = 100;
  m_triangleBatch.Create(BATCH_SIZE,
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
  const unsigned nTransforms = 2;

  unsigned modelID = m_currentTransformedModelID++;
  auto it = m_transformedModelsTable.emplace(
    std::make_pair<unsigned, ModelBatch>(std::move(modelID), {}));
  const bool insertedWithSuccess = it.second;
  if (!insertedWithSuccess) {
    return 0;
  }

  ModelBatch& model = it.first->second;
  model.Create({ vertexData, indices },
               nTransforms,
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
  auto it = m_transformedModelsTable.find(modelID);
  if (it == m_transformedModelsTable.end()) {
    return;
  }

  ModelBatch& model = it->second;

  model.Draw({ &transform, sizeof(glm::mat4x4) });
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
  for (auto& modelTableEntry : m_transformedModelsTable) {
    ModelBatch& model = modelTableEntry.second;
    model.Flush();
  }
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

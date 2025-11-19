#include "VertexColoredRendererImpl.h"

#include "CameraPrivate.h"
#include "Config.h"
#include "Shaders.h"

#include "Rendering/Shader.h"
#include "Rendering/ShaderAttribute.h"
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
  m_triangleMeshComponent.Init(Config::VertexColored::TriangleBatchSize(),
                               VERTEX_ATTRIBUTES,
                               [&]() { ConfigureShaderForTriangleDrawing(); });

  m_modelComponent.Init(Config::VertexColored::ModelsBatchSize(),
                        VERTEX_ATTRIBUTES,
                        TRANSFORMED_MODELS_INSTANCE_ATTRIBUTES,
                        [&]() { ConfigureShaderForTransformedModelDrawing(); });
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

  m_triangleMeshComponent.DrawTriangle({
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
  m_triangleMeshComponent.DrawTriangles(vertexData);
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::DrawTriangles(const std::vector<float>& vertexData,
                                         const std::vector<unsigned>& indexData)
{
  m_triangleMeshComponent.DrawTriangles(vertexData, indexData);
}

// ----------------------------------------------------------------------------

TriangleMeshID
VertexColoredRendererImpl::AddStaticTriangles(
  const std::vector<float>& vertexData)
{
  return m_triangleMeshComponent.AddStaticMesh(vertexData);
}

// ----------------------------------------------------------------------------

TriangleMeshID
VertexColoredRendererImpl::AddStaticTriangles(
  const std::vector<float>& vertexData,
  const std::vector<unsigned>& indexData)
{
  return m_triangleMeshComponent.AddStaticMesh(vertexData, indexData);
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::DeleteStaticTriangles(const TriangleMeshID& meshID)
{
  m_triangleMeshComponent.DeleteStaticMesh(meshID);
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::DrawModelTransformed(const ModelID& modelID,
                                                const glm::mat4x4& transform)
{
  m_modelComponent.DrawModel(modelID, { &transform, sizeof(glm::mat4x4) });
}

// ----------------------------------------------------------------------------

ModelInstanceID
VertexColoredRendererImpl::AddStaticModel(const ModelID& modelID,
                                          const glm::mat4x4& transform)
{
  return m_modelComponent.AddStaticModel(modelID,
                                         { &transform, sizeof(glm::mat4x4) });
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::DeleteStaticModel(const ModelInstanceID& instanceID)
{
  m_modelComponent.DeleteStaticModel(instanceID);
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::Flush()
{
  m_triangleMeshComponent.Flush();
  m_modelComponent.Flush();
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

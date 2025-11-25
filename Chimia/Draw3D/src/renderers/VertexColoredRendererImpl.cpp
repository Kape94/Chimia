#include "VertexColoredRendererImpl.h"

#include "CameraPrivate.h"
#include "Config.h"
#include "Core/Types.h"
#include "Draw3DPrivate.h"
#include "InternalTypes.h"
#include "Shaders.h"

#include "Rendering/Shader.h"
#include "Rendering/ShaderAttribute.h"
#include "Types.h"
#include "eRendererType.h"

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

constexpr unsigned MATERIAL_ID = 0;
constexpr unsigned RENDERER_ID =
  static_cast<unsigned>(eRendererType::VERTEX_COLORED);
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
VertexColoredRendererImpl::DrawTriangle(const Position3& p1,
                                        const Color3& color1,
                                        const Position3& p2,
                                        const Color3& color2,
                                        const Position3& p3,
                                        const Color3& color3)
{
  constexpr size_t POS3_SIZE = sizeof(Position3);
  constexpr size_t COL3_SIZE = sizeof(Color3);

  m_triangleMeshComponent.DrawTriangle({
    { &p1, POS3_SIZE },
    { &color1, COL3_SIZE },
    { &p2, POS3_SIZE },
    { &color2, COL3_SIZE },
    { &p3, POS3_SIZE },
    { &color3, COL3_SIZE },
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
  const unsigned instanceID = m_triangleMeshComponent.AddStaticMesh(vertexData);
  return Draw3DPrivate::CreateTriangleMeshID(
    RENDERER_ID, instanceID, MATERIAL_ID);
}

// ----------------------------------------------------------------------------

TriangleMeshID
VertexColoredRendererImpl::AddStaticTriangles(
  const std::vector<float>& vertexData,
  const std::vector<unsigned>& indexData)
{
  const unsigned instanceID =
    m_triangleMeshComponent.AddStaticMesh(vertexData, indexData);
  return Draw3DPrivate::CreateTriangleMeshID(
    RENDERER_ID, instanceID, MATERIAL_ID);
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::DeleteStaticTriangles(const TriangleMeshID& meshID)
{
  auto [rendererID, instanceID, materialID] =
    Draw3DPrivate::GetTriangleMeshIDValues(meshID);
  m_triangleMeshComponent.DeleteStaticMesh(instanceID);
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
  const LocalModelInstanceID localInstanceID = m_modelComponent.AddStaticModel(
    modelID, { &transform, sizeof(glm::mat4x4) });

  return Draw3DPrivate::CreateModelInstanceID(RENDERER_ID, localInstanceID);
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::DeleteStaticModel(const ModelInstanceID& instanceID)
{
  m_modelComponent.DeleteStaticModel(
    Draw3DPrivate::CreateLocalModelInstanceID(instanceID));
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

#include "LitWithVertexColorRendererImpl.h"

#include "Config.h"
#include "Core/Types.h"
#include "Draw3DPrivate.h"
#include "IlluminationPrivate.h"
#include "InternalTypes.h"
#include "Shaders.h"

#include "Rendering/Shader.h"
#include "Rendering/ShaderAttribute.h"
#include "TriangleMeshComponent.h"
#include "Types.h"
#include "eRendererType.h"
#include <glm/ext/vector_float3.hpp>

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace {

static const Chimia::Rendering::ShaderAttributes VERTEX_ATTRIBUTES{
  Chimia::Rendering::ShaderAttribute::Float(0 /*pos*/, 3),
  Chimia::Rendering::ShaderAttribute::Float(1 /*color*/, 3),
  Chimia::Rendering::ShaderAttribute::Float(2 /*normal*/, 3),
};

static const Chimia::Rendering::ShaderAttributes
  TRANSFORMED_MODELS_INSTANCE_ATTRIBUTES{
    Chimia::Rendering::ShaderAttribute::Float(3 /*transform*/, 4),
    Chimia::Rendering::ShaderAttribute::Float(4 /*transform*/, 4),
    Chimia::Rendering::ShaderAttribute::Float(5 /*transform*/, 4),
    Chimia::Rendering::ShaderAttribute::Float(6 /*transform*/, 4)
  };

Chimia::Rendering::Shader&
GetShaderForTriangleMeshDrawing()
{
  return Chimia::Draw3D::Config::Lit::IlluminationModel() ==
             eIlluminationModel::GOURAUD
           ? Chimia::Draw3D::Shaders::GouraudLitWithVertexColor()
           : Chimia::Draw3D::Shaders::PhongLitWithVertexColor();
}

Chimia::Rendering::Shader&
GetShaderForModelDrawing()
{
  return Config::Lit::IlluminationModel() == eIlluminationModel::GOURAUD
           ? Chimia::Draw3D::Shaders::
               GouraudLitWithInstancedTransformAndVertexColor()
           : Chimia::Draw3D::Shaders::
               PhongLitWithInstancedTransformAndVertexColor();
}

constexpr unsigned RENDERER_ID =
  static_cast<unsigned>(eRendererType::VERTEX_COLORED_LIT);
constexpr unsigned NO_MATERIAL = 0;
constexpr unsigned NO_TEXTURE = 0;
}

// ----------------------------------------------------------------------------

LitWithVertexColorRendererImpl&
LitWithVertexColorRendererImpl::getInstance()
{
  static LitWithVertexColorRendererImpl renderer;
  return renderer;
}

// ----------------------------------------------------------------------------

void
LitWithVertexColorRendererImpl::Init()
{
  m_modelComponent.Init(Config::Lit::ModelsBatchSize(),
                        VERTEX_ATTRIBUTES,
                        TRANSFORMED_MODELS_INSTANCE_ATTRIBUTES,
                        [&]() { ConfigureShaderForTransformedModelDrawing(); });

  m_triangleMeshComponent.Init(Config::Lit::TriangleBatchSizePerMaterial(),
                               VERTEX_ATTRIBUTES,
                               [&]() { ConfigureShaderForTriangleDrawing(); });
}

// ----------------------------------------------------------------------------

void
LitWithVertexColorRendererImpl::DrawTriangle(const glm::vec3& p1,
                                             const glm::vec3& p1Color,
                                             const glm::vec3& p1Normal,
                                             const glm::vec3& p2,
                                             const glm::vec3& p2Color,
                                             const glm::vec3& p2Normal,
                                             const glm::vec3& p3,
                                             const glm::vec3& p3Color,
                                             const glm::vec3& p3Normal)
{
  constexpr size_t POS3_SIZE = sizeof(glm::vec3);
  constexpr size_t COLOR3_SIZE = sizeof(glm::vec3);
  constexpr size_t NORM3_SIZE = sizeof(glm::vec3);

  m_triangleMeshComponent.DrawTriangle({
    { &p1, POS3_SIZE },
    { &p1Color, COLOR3_SIZE },
    { &p1Normal, NORM3_SIZE },
    { &p2, POS3_SIZE },
    { &p2Color, COLOR3_SIZE },
    { &p2Normal, NORM3_SIZE },
    { &p3, POS3_SIZE },
    { &p3Color, COLOR3_SIZE },
    { &p3Normal, NORM3_SIZE },
  });
}

// ----------------------------------------------------------------------------

void
LitWithVertexColorRendererImpl::DrawTriangles(
  const RawArrayView& vertexDataArray)
{
  m_triangleMeshComponent.DrawTriangles(vertexDataArray);
}

// ----------------------------------------------------------------------------

TriangleMeshID
LitWithVertexColorRendererImpl::AddStaticTriangles(
  const RawDataView& vertexData)
{
  const unsigned instanceID = m_triangleMeshComponent.AddStaticMesh(vertexData);

  return Draw3DPrivate::CreateTriangleMeshID(
    RENDERER_ID, instanceID, NO_MATERIAL, NO_TEXTURE);
}

// ----------------------------------------------------------------------------

void
LitWithVertexColorRendererImpl::DeleteStaticTriangles(
  const TriangleMeshID& meshID)
{
  auto [_, instanceIDValue, __, ___] =
    Draw3DPrivate::GetTriangleMeshIDValues(meshID);

  m_triangleMeshComponent.DeleteStaticMesh(instanceIDValue);
}

// ----------------------------------------------------------------------------

void
LitWithVertexColorRendererImpl::DrawModelTransformed(
  const ModelID& modelID,
  const glm::mat4x4& transform)
{
  m_modelComponent.DrawModel(modelID, { { &transform, sizeof(glm::mat4x4) } });
}

// ----------------------------------------------------------------------------

ModelInstanceID
LitWithVertexColorRendererImpl::AddStaticModel(const ModelID& modelID,
                                               const glm::mat4x4& transform)
{
  const LocalModelInstanceID localInstanceID = m_modelComponent.AddStaticModel(
    modelID, { { &transform, sizeof(glm::mat4x4) } });

  return Draw3DPrivate::CreateModelInstanceID(
    RENDERER_ID, localInstanceID, NO_MATERIAL);
}

// ----------------------------------------------------------------------------

void
LitWithVertexColorRendererImpl::DeleteStaticModel(
  const ModelInstanceID& instanceID)
{
  m_modelComponent.DeleteStaticModel(
    Draw3DPrivate::CreateLocalModelInstanceID(instanceID));
}

// ----------------------------------------------------------------------------

void
LitWithVertexColorRendererImpl::Flush()
{
  m_triangleMeshComponent.Flush();
  m_modelComponent.Flush();
}

// ----------------------------------------------------------------------------

void
LitWithVertexColorRendererImpl::ConfigureShaderForTriangleDrawing()
{
  Chimia::Rendering::Shader& shader = GetShaderForTriangleMeshDrawing();

  shader.Use();
  IlluminationPrivate::ConfigureLightsOnShader(shader);
}

// ----------------------------------------------------------------------------

void
LitWithVertexColorRendererImpl::ConfigureShaderForTransformedModelDrawing()
{
  Chimia::Rendering::Shader& shader = GetShaderForModelDrawing();

  shader.Use();
  IlluminationPrivate::ConfigureLightsOnShader(shader);
}

// ----------------------------------------------------------------------------

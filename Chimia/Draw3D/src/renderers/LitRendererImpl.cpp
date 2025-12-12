#include "LitRendererImpl.h"

#include "Config.h"
#include "Core/Types.h"
#include "Draw3DPrivate.h"
#include "IlluminationPrivate.h"
#include "InternalTypes.h"
#include "ResourcesManager.h"
#include "Shaders.h"

#include "Rendering/Shader.h"
#include "Rendering/ShaderAttribute.h"
#include "TriangleMeshComponent.h"
#include "Types.h"
#include "eRendererType.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace {

static const Chimia::Rendering::ShaderAttributes VERTEX_ATTRIBUTES{
  Chimia::Rendering::ShaderAttribute::Float(0 /*pos*/, 3),
  Chimia::Rendering::ShaderAttribute::Float(1 /*normal*/, 3)
};

static const Chimia::Rendering::ShaderAttributes
  TRANSFORMED_MODELS_INSTANCE_ATTRIBUTES{
    Chimia::Rendering::ShaderAttribute::Float(2 /*transform*/, 4),
    Chimia::Rendering::ShaderAttribute::Float(3 /*transform*/, 4),
    Chimia::Rendering::ShaderAttribute::Float(4 /*transform*/, 4),
    Chimia::Rendering::ShaderAttribute::Float(5 /*transform*/, 4),
    Chimia::Rendering::ShaderAttribute::Float(6 /*materialAmbient*/, 3),
    Chimia::Rendering::ShaderAttribute::Float(7 /*materialDiffuse*/, 3),
    Chimia::Rendering::ShaderAttribute::Float(8 /*materialSpecular*/, 3),
    Chimia::Rendering::ShaderAttribute::Float(9 /*materialShininess*/, 1)
  };

Chimia::Rendering::Shader&
GetShaderForTriangleMeshDrawing()
{
  return Chimia::Draw3D::Config::IlluminationModel() ==
             eIlluminationModel::GOURAUD
           ? Chimia::Draw3D::Shaders::GouraudLit()
           : Chimia::Draw3D::Shaders::PhongLit();
}

Chimia::Rendering::Shader&
GetShaderForModelDrawing()
{
  return Config::IlluminationModel() == eIlluminationModel::GOURAUD
           ? Chimia::Draw3D::Shaders::
               GouraudLitWithInstancedTransformAndMaterial()
           : Chimia::Draw3D::Shaders::
               PhongLitWithInstancedTransformAndMaterial();
}

constexpr unsigned RENDERER_ID = static_cast<unsigned>(eRendererType::LIT);
constexpr unsigned NO_TEXTURE = 0;
constexpr unsigned NO_RESOURCE_GROUP = 0;
}

// ----------------------------------------------------------------------------

LitRendererImpl&
LitRendererImpl::getInstance()
{
  static LitRendererImpl renderer;
  return renderer;
}

// ----------------------------------------------------------------------------

void
LitRendererImpl::Init()
{
  m_modelComponent.Init(Config::Batching::ModelBatchingSettings(),
                        VERTEX_ATTRIBUTES,
                        TRANSFORMED_MODELS_INSTANCE_ATTRIBUTES,
                        [&]() { ConfigureShaderForTransformedModelDrawing(); });
}

// ----------------------------------------------------------------------------

void
LitRendererImpl::DrawTriangle(const glm::vec3& p1,
                              const glm::vec3& p1Normal,
                              const glm::vec3& p2,
                              const glm::vec3& p2Normal,
                              const glm::vec3& p3,
                              const glm::vec3& p3Normal,
                              const MaterialID& materialID)
{
  constexpr size_t POS3_SIZE = sizeof(glm::vec3);
  constexpr size_t NORM3_SIZE = sizeof(glm::vec3);

  auto renderComponent = FetchTriangleRenderComponentForMaterial(materialID);
  renderComponent->DrawTriangle({
    { &p1, POS3_SIZE },
    { &p1Normal, NORM3_SIZE },
    { &p2, POS3_SIZE },
    { &p2Normal, NORM3_SIZE },
    { &p3, POS3_SIZE },
    { &p3Normal, NORM3_SIZE },
  });
}

// ----------------------------------------------------------------------------

void
LitRendererImpl::DrawTriangles(const RawArrayView& vertexDataArray,
                               const MaterialID& materialID)
{
  auto renderComponent = FetchTriangleRenderComponentForMaterial(materialID);
  renderComponent->DrawTriangles(vertexDataArray);
}

// ----------------------------------------------------------------------------

TriangleMeshID
LitRendererImpl::AddStaticTriangles(const RawDataView& vertexData,
                                    const MaterialID& materialID)
{
  auto renderComponent = FetchTriangleRenderComponentForMaterial(materialID);
  const unsigned instanceID = renderComponent->AddStaticMesh(vertexData);

  return Draw3DPrivate::CreateTriangleMeshID(
    RENDERER_ID,
    instanceID,
    Draw3DPrivate::GetMaterialIDValue(materialID),
    NO_TEXTURE,
    NO_RESOURCE_GROUP);
}

// ----------------------------------------------------------------------------

void
LitRendererImpl::DeleteStaticTriangles(const TriangleMeshID& meshID)
{
  auto [_, instanceIDValue, materialIDValue, __, ___] =
    Draw3DPrivate::GetTriangleMeshIDValues(meshID);

  auto renderComponent = FetchTriangleRenderComponentForMaterial(
    Draw3DPrivate::CreateMaterialID(materialIDValue));
  renderComponent->DeleteStaticMesh(instanceIDValue);
}

// ----------------------------------------------------------------------------

TriangleMeshComponent*
LitRendererImpl::FetchTriangleRenderComponentForMaterial(
  const MaterialID& materialID)
{
  const unsigned idValue = Draw3DPrivate::GetMaterialIDValue(materialID);
  auto renderComponent = m_triangleMeshComponents.Find(idValue);
  if (renderComponent == nullptr) {
    renderComponent = m_triangleMeshComponents.Insert(idValue);

    renderComponent->Init(
      Config::Batching::TriangleBatchingByResourceSettings(),
      VERTEX_ATTRIBUTES,
      [&]() { ConfigureShaderForTriangleDrawing(materialID); });
  }

  return renderComponent;
}

// ----------------------------------------------------------------------------

void
LitRendererImpl::DrawModelTransformed(const ModelID& modelID,
                                      const glm::mat4x4& transform,
                                      const MaterialID& materialID)
{
  auto material = ResourcesManager::GetInstance().GetMaterial(materialID);
  if (material == nullptr) {
    return;
  }

  m_modelComponent.DrawModel(modelID,
                             { { &transform, sizeof(glm::mat4x4) },
                               { &material->ambient, sizeof(glm::vec3) },
                               { &material->diffuse, sizeof(glm::vec3) },
                               { &material->specular, sizeof(glm::vec3) },
                               { &material->shininess, sizeof(float) } });
}

// ----------------------------------------------------------------------------

ModelInstanceID
LitRendererImpl::AddStaticModel(const ModelID& modelID,
                                const glm::mat4x4& transform,
                                const MaterialID& materialID)
{
  auto material = ResourcesManager::GetInstance().GetMaterial(materialID);
  if (material == nullptr) {
    return Draw3DPrivate::CreateModelInstanceID(0, 0, 0, 0, 0);
  }

  const LocalModelInstanceID localInstanceID = m_modelComponent.AddStaticModel(
    modelID,
    { { &transform, sizeof(glm::mat4x4) },
      { &material->ambient, sizeof(glm::vec3) },
      { &material->diffuse, sizeof(glm::vec3) },
      { &material->specular, sizeof(glm::vec3) },
      { &material->shininess, sizeof(float) } });

  return Draw3DPrivate::CreateModelInstanceID(
    RENDERER_ID, localInstanceID, NO_TEXTURE, NO_RESOURCE_GROUP);
}

// ----------------------------------------------------------------------------

void
LitRendererImpl::DeleteStaticModel(const ModelInstanceID& instanceID)
{
  m_modelComponent.DeleteStaticModel(
    Draw3DPrivate::CreateLocalModelInstanceID(instanceID));
}

// ----------------------------------------------------------------------------

void
LitRendererImpl::Flush()
{
  m_triangleMeshComponents.ForEach(
    [](TriangleMeshComponent& triangleRenderer) { triangleRenderer.Flush(); });
  m_modelComponent.Flush();
}

// ----------------------------------------------------------------------------

void
LitRendererImpl::ConfigureShaderForTriangleDrawing(const MaterialID& materialID)
{
  auto material = ResourcesManager::GetInstance().GetMaterial(materialID);
  if (material == nullptr) {
    return;
  }

  Chimia::Rendering::Shader& shader = GetShaderForTriangleMeshDrawing();

  shader.Use();
  IlluminationPrivate::ConfigureLightsOnShader(shader);
  IlluminationPrivate::ConfigureMaterialOnShader(*material, shader);
}

// ----------------------------------------------------------------------------

void
LitRendererImpl::ConfigureShaderForTransformedModelDrawing()
{
  Chimia::Rendering::Shader& shader = GetShaderForModelDrawing();

  shader.Use();
  IlluminationPrivate::ConfigureLightsOnShader(shader);
}

// ----------------------------------------------------------------------------

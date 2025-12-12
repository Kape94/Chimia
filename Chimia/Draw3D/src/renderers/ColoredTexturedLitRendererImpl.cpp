#include "ColoredTexturedLitRendererImpl.h"

#include "Config.h"
#include "Core/Types.h"
#include "Draw3DPrivate.h"
#include "IlluminationPrivate.h"
#include "InternalTypes.h"
#include "ModelRenderingComponent.h"
#include "ResourcesManager.h"
#include "Shaders.h"

#include "Rendering/Shader.h"
#include "Rendering/ShaderAttribute.h"
#include "Rendering/TextureUnit.h"
#include "TriangleMeshComponent.h"
#include "Types.h"
#include "eRendererType.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace {

static const Chimia::Rendering::ShaderAttributes VERTEX_ATTRIBUTES{
  Chimia::Rendering::ShaderAttribute::Float(0 /*pos*/, 3),
  Chimia::Rendering::ShaderAttribute::Float(1 /*color*/, 3),
  Chimia::Rendering::ShaderAttribute::Float(2 /*normal*/, 3),
  Chimia::Rendering::ShaderAttribute::Float(3 /*texCoord*/, 2)
};

static const Chimia::Rendering::ShaderAttributes
  TRANSFORMED_MODELS_INSTANCE_ATTRIBUTES{
    Chimia::Rendering::ShaderAttribute::Float(4 /*transform*/, 4),
    Chimia::Rendering::ShaderAttribute::Float(5 /*transform*/, 4),
    Chimia::Rendering::ShaderAttribute::Float(6 /*transform*/, 4),
    Chimia::Rendering::ShaderAttribute::Float(7 /*transform*/, 4)
  };

constexpr unsigned RENDERER_ID =
  static_cast<unsigned>(eRendererType::COLORED_TEXTURED_LIT);
constexpr unsigned NO_MATERIAL = 0;
constexpr unsigned NO_RESOURCE_GROUP = 0;

Chimia::Rendering::Shader&
GetShaderForTriangleMeshDrawing()
{
  return Chimia::Draw3D::Config::IlluminationModel() ==
             eIlluminationModel::GOURAUD
           ? Chimia::Draw3D::Shaders::GouraudLitColoredTextured()
           : Chimia::Draw3D::Shaders::PhongLitColoredTextured();
}

Chimia::Rendering::Shader&
GetShaderForModelDrawing()
{
  return Config::IlluminationModel() == eIlluminationModel::GOURAUD
           ? Chimia::Draw3D::Shaders::
               GouraudLitColoredTexturedWithInstancedTransform()
           : Chimia::Draw3D::Shaders::
               PhongLitColoredTexturedWithInstancedTransform();
}
}

// ----------------------------------------------------------------------------

ColoredTexturedLitRendererImpl&
ColoredTexturedLitRendererImpl::getInstance()
{
  static ColoredTexturedLitRendererImpl renderer;
  return renderer;
}

// ----------------------------------------------------------------------------

void
ColoredTexturedLitRendererImpl::Init()
{
}

// ----------------------------------------------------------------------------

void
ColoredTexturedLitRendererImpl::DrawTriangle(const glm::vec3& p1,
                                             const glm::vec3& p1Color,
                                             const glm::vec3& p1Normal,
                                             const glm::vec2& p1TexCoord,
                                             const glm::vec3& p2,
                                             const glm::vec3& p2Color,
                                             const glm::vec3& p2Normal,
                                             const glm::vec2& p2TexCoord,
                                             const glm::vec3& p3,
                                             const glm::vec3& p3Color,
                                             const glm::vec3& p3Normal,
                                             const glm::vec2& p3TexCoord,
                                             const TextureID& textureID)
{
  constexpr size_t POS3_SIZE = sizeof(glm::vec3);
  constexpr size_t COLOR3_SIZE = sizeof(glm::vec3);
  constexpr size_t TEX_COORD2_SIZE = sizeof(glm::vec2);
  constexpr size_t NORM3_SIZE = sizeof(glm::vec3);

  auto renderComponent = FetchTriangleRenderComponentForTexture(textureID);
  renderComponent->DrawTriangle({
    { &p1, POS3_SIZE },
    { &p1Color, COLOR3_SIZE },
    { &p1Normal, NORM3_SIZE },
    { &p1TexCoord, TEX_COORD2_SIZE },
    { &p2, POS3_SIZE },
    { &p2Color, COLOR3_SIZE },
    { &p2Normal, NORM3_SIZE },
    { &p2TexCoord, TEX_COORD2_SIZE },
    { &p3, POS3_SIZE },
    { &p3Color, COLOR3_SIZE },
    { &p3Normal, NORM3_SIZE },
    { &p3TexCoord, TEX_COORD2_SIZE },
  });
}

// ----------------------------------------------------------------------------

void
ColoredTexturedLitRendererImpl::DrawTriangles(
  const RawArrayView& vertexDataArray,
  const TextureID& textureID)
{
  auto renderComponent = FetchTriangleRenderComponentForTexture(textureID);
  renderComponent->DrawTriangles(vertexDataArray);
}

// ----------------------------------------------------------------------------

TriangleMeshID
ColoredTexturedLitRendererImpl::AddStaticTriangles(
  const RawDataView& vertexData,
  const TextureID& textureID)
{
  auto renderComponent = FetchTriangleRenderComponentForTexture(textureID);
  const unsigned instanceID = renderComponent->AddStaticMesh(vertexData);

  return Draw3DPrivate::CreateTriangleMeshID(
    RENDERER_ID,
    instanceID,
    NO_MATERIAL,
    Draw3DPrivate::GetTextureIDValue(textureID),
    NO_RESOURCE_GROUP);
}

// ----------------------------------------------------------------------------

void
ColoredTexturedLitRendererImpl::DeleteStaticTriangles(
  const TriangleMeshID& meshID)
{
  auto [_, instanceIDValue, __, textureIDValue, ___] =
    Draw3DPrivate::GetTriangleMeshIDValues(meshID);

  auto renderComponent = FetchTriangleRenderComponentForTexture(
    Draw3DPrivate::CreateTextureID(textureIDValue));
  renderComponent->DeleteStaticMesh(instanceIDValue);
}

// ----------------------------------------------------------------------------

TriangleMeshComponent*
ColoredTexturedLitRendererImpl::FetchTriangleRenderComponentForTexture(
  const TextureID& textureID)
{
  const unsigned idValue = Draw3DPrivate::GetTextureIDValue(textureID);
  auto renderComponent = m_triangleMeshComponents.Find(idValue);
  if (renderComponent == nullptr) {
    renderComponent = m_triangleMeshComponents.Insert(idValue);

    renderComponent->Init(
      Config::Batching::TriangleBatchingByResourceSettings(),
      VERTEX_ATTRIBUTES,
      [&]() { ConfigureShaderForTriangleDrawing(textureID); });
  }

  return renderComponent;
}

// ----------------------------------------------------------------------------

ModelRenderingComponent*
ColoredTexturedLitRendererImpl::FetchModelRenderComponentForTexture(
  const TextureID& textureID)
{
  const unsigned idValue = Draw3DPrivate::GetTextureIDValue(textureID);
  auto renderComponent = m_modelComponents.Find(idValue);
  if (renderComponent == nullptr) {
    renderComponent = m_modelComponents.Insert(idValue);

    renderComponent->Init(
      Config::Batching::ModelBatchingByResourceSettings(),
      VERTEX_ATTRIBUTES,
      TRANSFORMED_MODELS_INSTANCE_ATTRIBUTES,
      [&]() { ConfigureShaderForTransformedModelDrawing(textureID); });
  }

  return renderComponent;
}

// ----------------------------------------------------------------------------

void
ColoredTexturedLitRendererImpl::DrawModelTransformed(
  const ModelID& modelID,
  const glm::mat4x4& transform,
  const TextureID& textureID)
{
  auto modelComponent = FetchModelRenderComponentForTexture(textureID);
  modelComponent->DrawModel(modelID, { { &transform, sizeof(glm::mat4x4) } });
}

// ----------------------------------------------------------------------------

ModelInstanceID
ColoredTexturedLitRendererImpl::AddStaticModel(const ModelID& modelID,
                                               const glm::mat4x4& transform,
                                               const TextureID& textureID)
{
  auto modelComponent = FetchModelRenderComponentForTexture(textureID);

  const LocalModelInstanceID localInstanceID = modelComponent->AddStaticModel(
    modelID, { { &transform, sizeof(glm::mat4x4) } });

  return Draw3DPrivate::CreateModelInstanceID(
    RENDERER_ID,
    localInstanceID,
    Draw3DPrivate::GetTextureIDValue(textureID),
    NO_RESOURCE_GROUP);
}

// ----------------------------------------------------------------------------

void
ColoredTexturedLitRendererImpl::DeleteStaticModel(
  const ModelInstanceID& instanceID)
{
  auto [_, __, instanceIDValue, textureIDValue, ___] =
    Draw3DPrivate::GetModelInstanceIDValues(instanceID);

  const TextureID textureID = Draw3DPrivate::CreateTextureID(textureIDValue);
  auto modelComponent = FetchModelRenderComponentForTexture(textureID);

  modelComponent->DeleteStaticModel(
    Draw3DPrivate::CreateLocalModelInstanceID(instanceID));
}

// ----------------------------------------------------------------------------

void
ColoredTexturedLitRendererImpl::Flush()
{
  m_triangleMeshComponents.ForEach(
    [](TriangleMeshComponent& triangleRenderer) { triangleRenderer.Flush(); });
  m_modelComponents.ForEach(
    [](ModelRenderingComponent& modelRenderer) { modelRenderer.Flush(); });
}

// ----------------------------------------------------------------------------

void
ColoredTexturedLitRendererImpl::ConfigureShaderForTriangleDrawing(
  const TextureID& textureID)
{
  auto texture = ResourcesManager::GetInstance().GetTexture(textureID);
  if (texture == nullptr) {
    return;
  }

  Chimia::Rendering::Shader& shader = GetShaderForTriangleMeshDrawing();
  shader.Use();

  IlluminationPrivate::ConfigureLightsOnShader(shader);

  constexpr auto TEXTURE_UNIT = Rendering::TextureUnit::UNIT_1;

  texture->Use(TEXTURE_UNIT);
  shader.SetUniform("tex", TEXTURE_UNIT);
}

// ----------------------------------------------------------------------------

void
ColoredTexturedLitRendererImpl::ConfigureShaderForTransformedModelDrawing(
  const TextureID& textureID)
{
  auto texture = ResourcesManager::GetInstance().GetTexture(textureID);
  if (texture == nullptr) {
    return;
  }

  Chimia::Rendering::Shader& shader = GetShaderForModelDrawing();
  shader.Use();

  IlluminationPrivate::ConfigureLightsOnShader(shader);

  constexpr auto TEXTURE_UNIT = Rendering::TextureUnit::UNIT_1;

  texture->Use(TEXTURE_UNIT);
  shader.SetUniform("tex", TEXTURE_UNIT);
}

// ----------------------------------------------------------------------------

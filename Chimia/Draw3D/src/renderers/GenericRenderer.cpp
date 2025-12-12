#include "GenericRenderer.h"

#include "Config.h"
#include "Core/Types.h"
#include "Draw3DPrivate.h"
#include "InternalTypes.h"
#include "ModelRenderingComponent.h"
#include "ResourcesManager.h"
#include "Shaders.h"

#include "Rendering/Shader.h"
#include "Rendering/ShaderAttribute.h"
#include "TriangleMeshComponent.h"
#include "Types.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace {

constexpr unsigned NO_MATERIAL = 0;
constexpr unsigned NO_TEXTURE = 0;

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

GenericRenderer::GenericRenderer(
  const unsigned id,
  const Rendering::ShaderAttributes& vertexAttributes,
  const Rendering::ShaderAttributes& instancedAttributes,
  const void (*setupShaderForTriangleRendering)(const ResourcesGroup&),
  const void (*setupShaderForInstancedRendering)(const ResourcesGroup&))
  : m_id(id)
  , m_vertexAttributes(vertexAttributes)
  , m_instancedAttributes(instancedAttributes)
  , m_setupShaderForTriangleRendering(setupShaderForTriangleRendering)
  , m_setupShaderForInstancedRendering(setupShaderForInstancedRendering)
{
}

// ----------------------------------------------------------------------------

void
GenericRenderer::Init()
{
}

// ----------------------------------------------------------------------------

void
GenericRenderer::DrawTriangles(const RawArrayView& vertexDataArray,
                               const ResourceGroupID& resourceID)
{
  auto renderComponent = FetchTriangleRenderComponentForTexture(resourceID);
  renderComponent->DrawTriangles(vertexDataArray);
}

// ----------------------------------------------------------------------------

TriangleMeshID
GenericRenderer::AddStaticTriangles(const RawDataView& vertexData,
                                    const ResourceGroupID& resourceID)
{
  auto renderComponent = FetchTriangleRenderComponentForTexture(resourceID);
  const unsigned instanceID = renderComponent->AddStaticMesh(vertexData);

  return Draw3DPrivate::CreateTriangleMeshID(
    m_id,
    instanceID,
    NO_MATERIAL,
    NO_TEXTURE,
    Draw3DPrivate::GetResourceGroupIDValue(resourceID));
}

// ----------------------------------------------------------------------------

void
GenericRenderer::DeleteStaticTriangles(const TriangleMeshID& meshID)
{
  auto [_, instanceIDValue, __, ___, resourceID] =
    Draw3DPrivate::GetTriangleMeshIDValues(meshID);

  auto renderComponent = FetchTriangleRenderComponentForTexture(
    Draw3DPrivate::CreateResourceGroupID(resourceID));
  renderComponent->DeleteStaticMesh(instanceIDValue);
}

// ----------------------------------------------------------------------------

TriangleMeshComponent*
GenericRenderer::FetchTriangleRenderComponentForTexture(
  const ResourceGroupID& resourceID)
{
  const unsigned idValue = Draw3DPrivate::GetResourceGroupIDValue(resourceID);
  auto renderComponent = m_triangleMeshComponents.Find(idValue);
  if (renderComponent == nullptr) {
    renderComponent = m_triangleMeshComponents.Insert(idValue);

    renderComponent->Init(
      Config::Batching::TriangleBatchingByResourceSettings(),
      m_vertexAttributes,
      [&]() { ConfigureShaderForTriangleDrawing(resourceID); });
  }

  return renderComponent;
}

// ----------------------------------------------------------------------------

ModelRenderingComponent*
GenericRenderer::FetchModelRenderComponentForTexture(
  const ResourceGroupID& resourceID)
{
  const unsigned idValue = Draw3DPrivate::GetResourceGroupIDValue(resourceID);
  auto renderComponent = m_modelComponents.Find(idValue);
  if (renderComponent == nullptr) {
    renderComponent = m_modelComponents.Insert(idValue);

    renderComponent->Init(
      Config::Batching::ModelBatchingByResourceSettings(),
      m_vertexAttributes,
      m_instancedAttributes,
      [&]() { ConfigureShaderForTransformedModelDrawing(resourceID); });
  }

  return renderComponent;
}

// ----------------------------------------------------------------------------

void
GenericRenderer::DrawModelTransformed(const ModelID& modelID,
                                      const glm::mat4x4& transform,
                                      const ResourceGroupID& resourceID)
{
  auto modelComponent = FetchModelRenderComponentForTexture(resourceID);
  modelComponent->DrawModel(modelID, { { &transform, sizeof(glm::mat4x4) } });
}

// ----------------------------------------------------------------------------

ModelInstanceID
GenericRenderer::AddStaticModel(const ModelID& modelID,
                                const glm::mat4x4& transform,
                                const ResourceGroupID& resourceID)
{
  auto modelComponent = FetchModelRenderComponentForTexture(resourceID);

  const LocalModelInstanceID localInstanceID = modelComponent->AddStaticModel(
    modelID, { { &transform, sizeof(glm::mat4x4) } });

  return Draw3DPrivate::CreateModelInstanceID(
    m_id,
    localInstanceID,
    NO_TEXTURE,
    Draw3DPrivate::GetResourceGroupIDValue(resourceID));
}

// ----------------------------------------------------------------------------

void
GenericRenderer::DeleteStaticModel(const ModelInstanceID& instanceID)
{
  auto [_, __, instanceIDValue, ___, resourceIDValue] =
    Draw3DPrivate::GetModelInstanceIDValues(instanceID);

  const ResourceGroupID resourceID =
    Draw3DPrivate::CreateResourceGroupID(resourceIDValue);
  auto modelComponent = FetchModelRenderComponentForTexture(resourceID);

  modelComponent->DeleteStaticModel(
    Draw3DPrivate::CreateLocalModelInstanceID(instanceID));
}

// ----------------------------------------------------------------------------

void
GenericRenderer::Flush()
{
  m_triangleMeshComponents.ForEach(
    [](TriangleMeshComponent& triangleRenderer) { triangleRenderer.Flush(); });
  m_modelComponents.ForEach(
    [](ModelRenderingComponent& modelRenderer) { modelRenderer.Flush(); });
}

// ----------------------------------------------------------------------------

void
GenericRenderer::ConfigureShaderForTriangleDrawing(
  const ResourceGroupID& resourceID)
{
  auto resource = ResourcesManager::GetInstance().GetResourcesGroup(resourceID);
  if (resource == nullptr) {
    return;
  }

  m_setupShaderForTriangleRendering(*resource);
}

// ----------------------------------------------------------------------------

void
GenericRenderer::ConfigureShaderForTransformedModelDrawing(
  const ResourceGroupID& resourceID)
{
  auto resource = ResourcesManager::GetInstance().GetResourcesGroup(resourceID);
  if (resource == nullptr) {
    return;
  }

  m_setupShaderForInstancedRendering(*resource);
}

// ----------------------------------------------------------------------------

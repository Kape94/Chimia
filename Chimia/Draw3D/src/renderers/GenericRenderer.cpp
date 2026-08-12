#include "GenericRenderer.h"

#include "Config.h"
#include "Core/Types.h"
#include "Draw3DPrivate.h"
#include "InternalTypes.h"
#include "ModelRenderingComponent.h"
#include "ResourcesManager.h"

#include "TransitionRenderingComponent.h"
#include "TriangleMeshComponent.h"
#include "Types.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

void
GenericRenderer::Create(
  const unsigned id,
  const Rendering::DataLayout& vertexDataLayout,
  const Rendering::DataLayout& instancedDataLayout,
  const Rendering::DataLayout& transitionInstancedDataLayout,
  const ShaderBindingsTemplate& vertexBindingsTemplates,
  const ShaderBindingsTemplate& targetVertexBindingsTemplates,
  const ShaderBindingsTemplate& instancedBindingsTemplates,
  const ShaderBindingsTemplate& transitionInstancedBindingsTemplates,
  void (*setupShaderForTriangleRendering)(const ResourcesGroup&),
  void (*setupShaderForInstancedRendering)(const ResourcesGroup&),
  void (*setupShaderForTransitionRendering)(const ResourcesGroup&))
{
  m_id = id;
  m_vertexDataLayout = vertexDataLayout;
  m_instancedDataLayout = instancedDataLayout;
  m_transitionInstancedDataLayout = transitionInstancedDataLayout;
  m_vertexBindingsTemplates = vertexBindingsTemplates;
  m_targetVertexBindingsTemplates = targetVertexBindingsTemplates;
  m_instancedBindingsTemplates = instancedBindingsTemplates;
  m_transitionInstancedBindingsTemplates = transitionInstancedBindingsTemplates;
  m_setupShaderForTriangleRendering = setupShaderForTriangleRendering;
  m_setupShaderForInstancedRendering = setupShaderForInstancedRendering;
  m_setupShaderForTransitionRendering = setupShaderForTransitionRendering;
}

// ----------------------------------------------------------------------------

void
GenericRenderer::Init()
{
}

// ----------------------------------------------------------------------------

void
GenericRenderer::DrawTriangle(
  const std::initializer_list<RawDataView>& vertexData,
  const ResourceGroupID& resourcesID)
{
  auto renderComponent = FetchTriangleRenderComponentForResource(resourcesID);
  renderComponent->DrawTriangle(vertexData);
}

// ----------------------------------------------------------------------------

void
GenericRenderer::DrawTriangles(const RawArrayView& vertexDataArray,
                               const ResourceGroupID& resourceID)
{
  auto renderComponent = FetchTriangleRenderComponentForResource(resourceID);
  renderComponent->DrawTriangles(vertexDataArray);
}

// ----------------------------------------------------------------------------

TriangleMeshID
GenericRenderer::AddRetainedTriangles(const RawDataView& vertexData,
                                      const ResourceGroupID& resourceID)
{
  auto renderComponent = FetchTriangleRenderComponentForResource(resourceID);
  const unsigned instanceID = renderComponent->AddRetainedMesh(vertexData);

  return Draw3DPrivate::CreateTriangleMeshID(
    m_id, instanceID, Draw3DPrivate::GetResourceGroupIDValue(resourceID));
}

// ----------------------------------------------------------------------------

void
GenericRenderer::DeleteRetainedTriangles(const TriangleMeshID& meshID)
{
  auto [_, instanceIDValue, resourceID] =
    Draw3DPrivate::GetTriangleMeshIDValues(meshID);

  auto renderComponent = FetchTriangleRenderComponentForResource(
    Draw3DPrivate::CreateResourceGroupID(resourceID));
  renderComponent->DeleteRetainedMesh(instanceIDValue);
}

// ----------------------------------------------------------------------------

TriangleMeshComponent*
GenericRenderer::FetchTriangleRenderComponentForResource(
  const ResourceGroupID& resourceID)
{
  const unsigned idValue = Draw3DPrivate::GetResourceGroupIDValue(resourceID);
  auto renderComponent = m_triangleMeshComponents.Find(idValue);
  if (renderComponent == nullptr) {
    renderComponent = m_triangleMeshComponents.InsertWithID(idValue);

    renderComponent->Init(
      Config::Batching::TriangleBatchingByResourceSettings(),
      m_vertexDataLayout,
      m_vertexBindingsTemplates,
      [&, resourceID]() { ConfigureShaderForTriangleDrawing(resourceID); });
  }

  return renderComponent;
}

// ----------------------------------------------------------------------------

ModelRenderingComponent*
GenericRenderer::FetchModelRenderComponentForResource(
  const ResourceGroupID& resourceID)
{
  const unsigned idValue = Draw3DPrivate::GetResourceGroupIDValue(resourceID);
  auto renderComponent = m_modelComponents.Find(idValue);
  if (renderComponent == nullptr) {
    renderComponent = m_modelComponents.InsertWithID(idValue);

    renderComponent->Init(Config::Batching::ModelBatchingByResourceSettings(),
                          m_instancedDataLayout,
                          m_vertexBindingsTemplates,
                          m_instancedBindingsTemplates,
                          [&, resourceID]() {
                            ConfigureShaderForTransformedModelDrawing(
                              resourceID);
                          });
  }

  return renderComponent;
}

// ----------------------------------------------------------------------------

TransitionRenderingComponent*
GenericRenderer::FetchTransitionRenderComponentForResource(
  const ResourceGroupID& resourceID)
{
  const unsigned idValue = Draw3DPrivate::GetResourceGroupIDValue(resourceID);
  auto renderComponent = m_transitionComponents.Find(idValue);
  if (renderComponent == nullptr) {
    renderComponent = m_transitionComponents.InsertWithID(idValue);

    renderComponent->Init(
      Config::Batching::ModelBatchingByResourceSettings(),
      m_transitionInstancedDataLayout,
      m_vertexBindingsTemplates,
      m_targetVertexBindingsTemplates,
      m_transitionInstancedBindingsTemplates,
      [&, resourceID]() { ConfigureShaderForTransitionDrawing(resourceID); });
  }

  return renderComponent;
}

// ----------------------------------------------------------------------------

void
GenericRenderer::DrawModelTransformed(const ModelID& modelID,
                                      const glm::mat4x4& transform,
                                      const ResourceGroupID& resourceID)
{
  auto modelComponent = FetchModelRenderComponentForResource(resourceID);
  modelComponent->DrawModel(modelID, { { &transform, sizeof(glm::mat4x4) } });
}

// ----------------------------------------------------------------------------

ModelInstanceID
GenericRenderer::AddRetainedModel(const ModelID& modelID,
                                  const glm::mat4x4& transform,
                                  const ResourceGroupID& resourceID)
{
  auto modelComponent = FetchModelRenderComponentForResource(resourceID);

  const LocalModelInstanceID localInstanceID = modelComponent->AddRetainedModel(
    modelID, { { &transform, sizeof(glm::mat4x4) } });

  return Draw3DPrivate::CreateModelInstanceID(
    m_id, localInstanceID, Draw3DPrivate::GetResourceGroupIDValue(resourceID));
}

// ----------------------------------------------------------------------------

void
GenericRenderer::DeleteRetainedModel(const ModelInstanceID& instanceID)
{
  auto [_, __, ___, resourceIDValue] =
    Draw3DPrivate::GetModelInstanceIDValues(instanceID);

  const ResourceGroupID resourceID =
    Draw3DPrivate::CreateResourceGroupID(resourceIDValue);
  auto modelComponent = FetchModelRenderComponentForResource(resourceID);

  modelComponent->DeleteRetainedModel(
    Draw3DPrivate::CreateLocalModelInstanceID(instanceID));
}

// ----------------------------------------------------------------------------

void
GenericRenderer::DrawTransition(const TransitionID& transition,
                                const float interpolation,
                                const glm::mat4x4& transform,
                                const ResourceGroupID& resource)
{
  auto transitionComponent =
    FetchTransitionRenderComponentForResource(resource);
  transitionComponent->DrawTransition(
    transition,
    { { &transform, sizeof(glm::mat4x4) }, { &interpolation, sizeof(float) } });
}

// ----------------------------------------------------------------------------

void
GenericRenderer::Flush(const eImmediateFlusingPolicy flushingPolicy)
{
  m_triangleMeshComponents.ForEach(
    [flushingPolicy](TriangleMeshComponent& triangleRenderer) {
      triangleRenderer.Flush(flushingPolicy);
    });
  m_modelComponents.ForEach(
    [flushingPolicy](ModelRenderingComponent& modelRenderer) {
      modelRenderer.Flush(flushingPolicy);
    });
  m_transitionComponents.ForEach(
    [flushingPolicy](TransitionRenderingComponent& transitionRenderer) {
      transitionRenderer.Flush(flushingPolicy);
    });
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

void
GenericRenderer::ConfigureShaderForTransitionDrawing(
  const ResourceGroupID& resourceID)
{
  auto resource = ResourcesManager::GetInstance().GetResourcesGroup(resourceID);
  if (resource == nullptr) {
    return;
  }

  m_setupShaderForTransitionRendering(*resource);
}

// ----------------------------------------------------------------------------
#include "ModelRendering.h"

#include "Draw3DPrivate.h"
#include "LitRendererImpl.h"
#include "ResourcesManager.h"
#include "Types.h"
#include "VertexColoredRendererImpl.h"
#include "eRendererType.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace {
auto& renderer = VertexColoredRendererImpl::getInstance();
auto& litRenderer = LitRendererImpl::getInstance();
}

// ----------------------------------------------------------------------------

void
CHIMIA_DRAW3D_NAMESPACE_NAME::DrawModel(const ModelID& modelID,
                                        const glm::mat4x4& transform)
{
  assert(ResourcesManager::GetInstance().GetModel(modelID)->GetVertexLayout() ==
         eVertexLayout::POSITION3_COLOR3);

  renderer.DrawModelTransformed(modelID, transform);
}

// ----------------------------------------------------------------------------

ModelInstanceID
CHIMIA_DRAW3D_NAMESPACE_NAME::AddStaticModel(const ModelID& modelID,
                                             const glm::mat4x4& transform)
{
  assert(ResourcesManager::GetInstance().GetModel(modelID)->GetVertexLayout() ==
         eVertexLayout::POSITION3_COLOR3);

  return renderer.AddStaticModel(modelID, transform);
}

// ----------------------------------------------------------------------------

void
CHIMIA_DRAW3D_NAMESPACE_NAME::DrawModel(const ModelID& modelID,
                                        const glm::mat4x4& transform,
                                        const MaterialID& materialID)
{
  assert(ResourcesManager::GetInstance().GetModel(modelID)->GetVertexLayout() ==
         eVertexLayout::POSITION3_NORMAL3);

  litRenderer.DrawModelTransformed(modelID, transform, materialID);
}

// ----------------------------------------------------------------------------

ModelInstanceID
CHIMIA_DRAW3D_NAMESPACE_NAME::AddStaticModel(const ModelID& modelID,
                                             const glm::mat4x4& transform,
                                             const MaterialID& materialID)
{
  assert(ResourcesManager::GetInstance().GetModel(modelID)->GetVertexLayout() ==
         eVertexLayout::POSITION3_NORMAL3);

  return litRenderer.AddStaticModel(modelID, transform, materialID);
}

// ----------------------------------------------------------------------------

void
CHIMIA_DRAW3D_NAMESPACE_NAME::DeleteStaticModel(
  const ModelInstanceID& instanceID)
{
  auto [rendererID, _, __] =
    Draw3DPrivate::GetModelInstanceIDValues(instanceID);

  const auto rendererType = static_cast<eRendererType>(rendererID);
  switch (rendererType) {
    case eRendererType::VERTEX_COLORED: {
      renderer.DeleteStaticModel(instanceID);
      return;
    }
    case eRendererType::LIT: {
      litRenderer.DeleteStaticModel(instanceID);
      return;
    }
    case eRendererType::NONE:
    default:
      return;
  }
}

// ----------------------------------------------------------------------------

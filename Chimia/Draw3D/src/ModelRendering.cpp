#include "ModelRendering.h"

#include "Draw3DPrivate.h"
#include "LitRendererImpl.h"
#include "LitWithVertexColorRendererImpl.h"
#include "ResourcesManager.h"
#include "TexturedLitRendererImpl.h"
#include "TexturedRendererImpl.h"
#include "Types.h"
#include "VertexColoredRendererImpl.h"
#include "eRendererType.h"
#include <cassert>

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace {
auto& renderer = VertexColoredRendererImpl::getInstance();
auto& litRenderer = LitRendererImpl::getInstance();
auto& texturedRenderer = TexturedRendererImpl::getInstance();
auto& texturedLitRenderer = TexturedLitRendererImpl::getInstance();
auto& litVertexColoredRenderer = LitWithVertexColorRendererImpl::getInstance();

eVertexLayout
ModelLayout(const ModelID& modelID)
{
  return ResourcesManager::GetInstance().GetModel(modelID)->GetVertexLayout();
}

}

// ----------------------------------------------------------------------------
// Position3 + Color3
// Position3 + Color3 + Normal3
// ----------------------------------------------------------------------------

void
CHIMIA_DRAW3D_NAMESPACE_NAME::DrawModel(const ModelID& modelID,
                                        const glm::mat4x4& transform)
{
  const eVertexLayout layout = ModelLayout(modelID);
  switch (layout) {
    case eVertexLayout::POSITION3_COLOR3: {
      renderer.DrawModelTransformed(modelID, transform);
      break;
    }
    case eVertexLayout::POSITION3_COLOR3_NORMAL3: {
      litVertexColoredRenderer.DrawModelTransformed(modelID, transform);
      break;
    }
    default:
      assert(false && "Unsuported model layout");
  }
}

// ----------------------------------------------------------------------------

ModelInstanceID
CHIMIA_DRAW3D_NAMESPACE_NAME::AddStaticModel(const ModelID& modelID,
                                             const glm::mat4x4& transform)
{
  const eVertexLayout layout = ModelLayout(modelID);
  switch (layout) {
    case eVertexLayout::POSITION3_COLOR3: {
      return renderer.AddStaticModel(modelID, transform);
    }
    case eVertexLayout::POSITION3_COLOR3_NORMAL3: {
      return litVertexColoredRenderer.AddStaticModel(modelID, transform);
    }
    default:
      assert(false && "Unsuported model layout");
      return Draw3DPrivate::CreateModelInstanceID(0, 0, 0, 0);
  }
}

// ----------------------------------------------------------------------------
// Position3 + Normal3
// ----------------------------------------------------------------------------

void
CHIMIA_DRAW3D_NAMESPACE_NAME::DrawModel(const ModelID& modelID,
                                        const glm::mat4x4& transform,
                                        const MaterialID& materialID)
{
  assert(ModelLayout(modelID) == eVertexLayout::POSITION3_NORMAL3);

  litRenderer.DrawModelTransformed(modelID, transform, materialID);
}

// ----------------------------------------------------------------------------

ModelInstanceID
CHIMIA_DRAW3D_NAMESPACE_NAME::AddStaticModel(const ModelID& modelID,
                                             const glm::mat4x4& transform,
                                             const MaterialID& materialID)
{
  assert(ModelLayout(modelID) == eVertexLayout::POSITION3_NORMAL3);

  return litRenderer.AddStaticModel(modelID, transform, materialID);
}

// ----------------------------------------------------------------------------
// Position3 + TexCoord2
// Position3 + Normal3 + TexCoord2
// ----------------------------------------------------------------------------

void
CHIMIA_DRAW3D_NAMESPACE_NAME::DrawModel(const ModelID& modelID,
                                        const glm::mat4x4& transform,
                                        const TextureID& textureID)
{
  const eVertexLayout layout = ModelLayout(modelID);
  switch (layout) {
    case eVertexLayout::POSITION3_TEXCOORD2: {
      texturedRenderer.DrawModelTransformed(modelID, transform, textureID);
      break;
    }
    case eVertexLayout::POSITION3_NORMAL3_TEXCOORD2: {
      texturedLitRenderer.DrawModelTransformed(modelID, transform, textureID);
      break;
    }
    default:
      assert(false && "Unsuported model layout");
  }
}

// ----------------------------------------------------------------------------

ModelInstanceID
CHIMIA_DRAW3D_NAMESPACE_NAME::AddStaticModel(const ModelID& modelID,
                                             const glm::mat4x4& transform,
                                             const TextureID& textureID)
{
  const eVertexLayout layout = ModelLayout(modelID);
  switch (layout) {
    case eVertexLayout::POSITION3_TEXCOORD2: {
      return texturedRenderer.AddStaticModel(modelID, transform, textureID);
    }
    case eVertexLayout::POSITION3_NORMAL3_TEXCOORD2: {
      return texturedLitRenderer.AddStaticModel(modelID, transform, textureID);
    }
    default:
      assert(false && "Unsuported model layout");
      return Draw3DPrivate::CreateModelInstanceID(0, 0, 0, 0);
  }
}

// ----------------------------------------------------------------------------
// General
// ----------------------------------------------------------------------------

void
CHIMIA_DRAW3D_NAMESPACE_NAME::DeleteStaticModel(
  const ModelInstanceID& instanceID)
{
  auto [rendererID, _, __, ___] =
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
    case eRendererType::TEXTURED: {
      texturedRenderer.DeleteStaticModel(instanceID);
      return;
    }
    case eRendererType::VERTEX_COLORED_LIT: {
      litVertexColoredRenderer.DeleteStaticModel(instanceID);
      return;
    }
    case eRendererType::TEXTURED_LIT: {
      texturedLitRenderer.DeleteStaticModel(instanceID);
      return;
    }
    case eRendererType::NONE:
    default:
      return;
  }
}

// ----------------------------------------------------------------------------

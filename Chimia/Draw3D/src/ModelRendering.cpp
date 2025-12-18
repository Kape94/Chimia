#include "ModelRendering.h"

#include "ColoredTexturedLitRendererImpl.h"
#include "ColoredTexturedRendererImpl.h"
#include "Draw3DPrivate.h"
#include "GenericRenderer.h"
#include "LitRendererImpl.h"
#include "LitWithVertexColorRendererImpl.h"
#include "ModelRenderingPrivate.h"
#include "Renderers.h"
#include "ResourceGroupHelper.h"
#include "ResourcesManager.h"
#include "TexturedLitRendererImpl.h"
#include "TexturedRendererImpl.h"
#include "Types.h"
#include "VertexColoredRendererImpl.h"
#include <cassert>

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace {
GenericRenderer* renderer = nullptr;
GenericRenderer* litRenderer = nullptr;
GenericRenderer* texturedRenderer = nullptr;
GenericRenderer* texturedLitRenderer = nullptr;
GenericRenderer* litVertexColoredRenderer = nullptr;
GenericRenderer* coloredTexturedRenderer = nullptr;
GenericRenderer* coloredTexturedLitRenderer = nullptr;

eVertexLayout
ModelLayout(const ModelID& modelID)
{
  return ResourcesManager::GetInstance().GetModel(modelID)->GetVertexLayout();
}

}

// ----------------------------------------------------------------------------
// ModelRenderingPrivate
// ----------------------------------------------------------------------------

void
ModelRenderingPrivate::Init()
{
  renderer = &VertexColoredRendererImpl::GetRenderer();
  litRenderer = &LitRendererImpl::GetRenderer();
  texturedRenderer = &TexturedRendererImpl::GetRenderer();
  texturedLitRenderer = &TexturedLitRendererImpl::GetRenderer();
  litVertexColoredRenderer = &LitWithVertexColorRendererImpl::GetRenderer();
  coloredTexturedRenderer = &ColoredTexturedRendererImpl::GetRenderer();
  coloredTexturedLitRenderer = &ColoredTexturedLitRendererImpl::GetRenderer();
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
      renderer->DrawModelTransformed(
        modelID, transform, ResourceGroupHelper::GetEmptyResource());
      break;
    }
    case eVertexLayout::POSITION3_COLOR3_NORMAL3: {
      litVertexColoredRenderer->DrawModelTransformed(
        modelID, transform, ResourceGroupHelper::GetEmptyResource());
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
      return renderer->AddStaticModel(
        modelID, transform, ResourceGroupHelper::GetEmptyResource());
    }
    case eVertexLayout::POSITION3_COLOR3_NORMAL3: {
      return litVertexColoredRenderer->AddStaticModel(
        modelID, transform, ResourceGroupHelper::GetEmptyResource());
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

  litRenderer->DrawModelTransformed(
    modelID, transform, ResourceGroupHelper::GetResourceGroup(materialID));
}

// ----------------------------------------------------------------------------

ModelInstanceID
CHIMIA_DRAW3D_NAMESPACE_NAME::AddStaticModel(const ModelID& modelID,
                                             const glm::mat4x4& transform,
                                             const MaterialID& materialID)
{
  assert(ModelLayout(modelID) == eVertexLayout::POSITION3_NORMAL3);

  return litRenderer->AddStaticModel(
    modelID, transform, ResourceGroupHelper::GetResourceGroup(materialID));
}

// ----------------------------------------------------------------------------
// Position3 + TexCoord2
// Position3 + Color3 + TexCoord2
// Position3 + Normal3 + TexCoord2
// Position3 + Color3 + Normal3 + TexCoord2
// ----------------------------------------------------------------------------

void
CHIMIA_DRAW3D_NAMESPACE_NAME::DrawModel(const ModelID& modelID,
                                        const glm::mat4x4& transform,
                                        const TextureID& textureID)
{
  const eVertexLayout layout = ModelLayout(modelID);
  switch (layout) {
    case eVertexLayout::POSITION3_TEXCOORD2: {
      texturedRenderer->DrawModelTransformed(
        modelID, transform, ResourceGroupHelper::GetResourceGroup(textureID));
      break;
    }
    case eVertexLayout::POSITION3_NORMAL3_TEXCOORD2: {
      texturedLitRenderer->DrawModelTransformed(
        modelID, transform, ResourceGroupHelper::GetResourceGroup(textureID));
      break;
    }
    case eVertexLayout::POSITION3_COLOR3_TEXCOORD2: {
      coloredTexturedRenderer->DrawModelTransformed(
        modelID, transform, ResourceGroupHelper::GetResourceGroup(textureID));
      break;
    }
    case eVertexLayout::POSITION3_COLOR3_NORMAL3_TEXCOORD2: {
      coloredTexturedLitRenderer->DrawModelTransformed(
        modelID, transform, ResourceGroupHelper::GetResourceGroup(textureID));
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
      return texturedRenderer->AddStaticModel(
        modelID, transform, ResourceGroupHelper::GetResourceGroup(textureID));
    }
    case eVertexLayout::POSITION3_NORMAL3_TEXCOORD2: {
      return texturedLitRenderer->AddStaticModel(
        modelID, transform, ResourceGroupHelper::GetResourceGroup(textureID));
    }
    case eVertexLayout::POSITION3_COLOR3_TEXCOORD2: {
      return coloredTexturedRenderer->AddStaticModel(
        modelID, transform, ResourceGroupHelper::GetResourceGroup(textureID));
    }
    case eVertexLayout::POSITION3_COLOR3_NORMAL3_TEXCOORD2: {
      return coloredTexturedLitRenderer->AddStaticModel(
        modelID, transform, ResourceGroupHelper::GetResourceGroup(textureID));
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

  if (GenericRenderer* renderer = Renderers::GetRendererByID(rendererID)) {
    renderer->DeleteStaticModel(instanceID);
  }
}

// ----------------------------------------------------------------------------

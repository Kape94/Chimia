#include "ModelRendering.h"

#include "Draw3DPrivate.h"
#include "GenericRenderer.h"
#include "ModelRenderingPrivate.h"
#include "Renderers.h"
#include "ResourceGroupHelper.h"
#include "ResourcesManager.h"
#include "Types.h"

#include "defaultRenderers/Color3.h"
#include "defaultRenderers/Color3Normal3.h"
#include "defaultRenderers/Color3Normal3TexCoord2.h"
#include "defaultRenderers/Color3TexCoord2.h"
#include "defaultRenderers/Normal3.h"
#include "defaultRenderers/Normal3TexCoord2.h"
#include "defaultRenderers/TexCoord2.h"

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
  renderer = &DefaultRenderers::Color3::GetRenderer();
  litRenderer = &DefaultRenderers::Normal3::GetRenderer();
  texturedRenderer = &DefaultRenderers::TexCoord2::GetRenderer();
  texturedLitRenderer = &DefaultRenderers::Normal3TexCoord2::GetRenderer();
  litVertexColoredRenderer = &DefaultRenderers::Color3Normal3::GetRenderer();
  coloredTexturedRenderer = &DefaultRenderers::Color3TexCoord2::GetRenderer();
  coloredTexturedLitRenderer =
    &DefaultRenderers::Color3Normal3TexCoord2::GetRenderer();
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

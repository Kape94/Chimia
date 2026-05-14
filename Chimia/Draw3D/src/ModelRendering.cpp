#include "ModelRendering.h"

#include "Draw3DPrivate.h"
#include "GenericRenderer.h"
#include "ModelRenderingPrivate.h"
#include "Renderers.h"
#include "ResourceGroupHelper.h"
#include "ResourcesManager.h"
#include "Types.h"

#include "defaultRenderers/Color4.h"
#include "defaultRenderers/Color4Normal3.h"
#include "defaultRenderers/Color4Normal3TexCoord2.h"
#include "defaultRenderers/Color4TexCoord2.h"
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
  renderer = &DefaultRenderers::Color4::GetRenderer();
  litRenderer = &DefaultRenderers::Normal3::GetRenderer();
  texturedRenderer = &DefaultRenderers::TexCoord2::GetRenderer();
  texturedLitRenderer = &DefaultRenderers::Normal3TexCoord2::GetRenderer();
  litVertexColoredRenderer = &DefaultRenderers::Color4Normal3::GetRenderer();
  coloredTexturedRenderer = &DefaultRenderers::Color4TexCoord2::GetRenderer();
  coloredTexturedLitRenderer =
    &DefaultRenderers::Color4Normal3TexCoord2::GetRenderer();
}

// ----------------------------------------------------------------------------
// Position3 + Color4
// Position3 + Color4 + Normal3
// ----------------------------------------------------------------------------

void
CHIMIA_DRAW3D_NAMESPACE_NAME::DrawModel(const ModelID& modelID,
                                        const glm::mat4x4& transform)
{
  const eVertexLayout layout = ModelLayout(modelID);
  switch (layout) {
    case eVertexLayout::POSITION3_COLOR4: {
      renderer->DrawModelTransformed(
        modelID, transform, ResourceGroupHelper::GetEmptyResource());
      break;
    }
    case eVertexLayout::POSITION3_COLOR4_NORMAL3: {
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
CHIMIA_DRAW3D_NAMESPACE_NAME::AddRetainedModel(const ModelID& modelID,
                                               const glm::mat4x4& transform)
{
  const eVertexLayout layout = ModelLayout(modelID);
  switch (layout) {
    case eVertexLayout::POSITION3_COLOR4: {
      return renderer->AddRetainedModel(
        modelID, transform, ResourceGroupHelper::GetEmptyResource());
    }
    case eVertexLayout::POSITION3_COLOR4_NORMAL3: {
      return litVertexColoredRenderer->AddRetainedModel(
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
CHIMIA_DRAW3D_NAMESPACE_NAME::AddRetainedModel(const ModelID& modelID,
                                               const glm::mat4x4& transform,
                                               const MaterialID& materialID)
{
  assert(ModelLayout(modelID) == eVertexLayout::POSITION3_NORMAL3);

  return litRenderer->AddRetainedModel(
    modelID, transform, ResourceGroupHelper::GetResourceGroup(materialID));
}

// ----------------------------------------------------------------------------
// Position3 + TexCoord2
// Position3 + Color4 + TexCoord2
// Position3 + Normal3 + TexCoord2
// Position3 + Color4 + Normal3 + TexCoord2
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
    case eVertexLayout::POSITION3_COLOR4_TEXCOORD2: {
      coloredTexturedRenderer->DrawModelTransformed(
        modelID, transform, ResourceGroupHelper::GetResourceGroup(textureID));
      break;
    }
    case eVertexLayout::POSITION3_COLOR4_NORMAL3_TEXCOORD2: {
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
CHIMIA_DRAW3D_NAMESPACE_NAME::AddRetainedModel(const ModelID& modelID,
                                               const glm::mat4x4& transform,
                                               const TextureID& textureID)
{
  const eVertexLayout layout = ModelLayout(modelID);
  switch (layout) {
    case eVertexLayout::POSITION3_TEXCOORD2: {
      return texturedRenderer->AddRetainedModel(
        modelID, transform, ResourceGroupHelper::GetResourceGroup(textureID));
    }
    case eVertexLayout::POSITION3_NORMAL3_TEXCOORD2: {
      return texturedLitRenderer->AddRetainedModel(
        modelID, transform, ResourceGroupHelper::GetResourceGroup(textureID));
    }
    case eVertexLayout::POSITION3_COLOR4_TEXCOORD2: {
      return coloredTexturedRenderer->AddRetainedModel(
        modelID, transform, ResourceGroupHelper::GetResourceGroup(textureID));
    }
    case eVertexLayout::POSITION3_COLOR4_NORMAL3_TEXCOORD2: {
      return coloredTexturedLitRenderer->AddRetainedModel(
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
CHIMIA_DRAW3D_NAMESPACE_NAME::DeleteRetainedModel(
  const ModelInstanceID& instanceID)
{
  auto [rendererID, _, __, ___] =
    Draw3DPrivate::GetModelInstanceIDValues(instanceID);

  if (GenericRenderer* renderer = Renderers::GetRendererByID(rendererID)) {
    renderer->DeleteRetainedModel(instanceID);
  }
}

// ----------------------------------------------------------------------------

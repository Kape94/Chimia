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
GenericRenderer* color4Renderer = nullptr;
GenericRenderer* normal3Renderer = nullptr;
GenericRenderer* texCoord2Renderer = nullptr;
GenericRenderer* normal3TexCoord2Renderer = nullptr;
GenericRenderer* color4Normal3Renderer = nullptr;
GenericRenderer* color4TexCoord2Renderer = nullptr;
GenericRenderer* color4Normal3TexCoord2Renderer = nullptr;

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
  color4Renderer = &DefaultRenderers::Color4::GetRenderer();
  normal3Renderer = &DefaultRenderers::Normal3::GetRenderer();
  texCoord2Renderer = &DefaultRenderers::TexCoord2::GetRenderer();
  normal3TexCoord2Renderer = &DefaultRenderers::Normal3TexCoord2::GetRenderer();
  color4Normal3Renderer = &DefaultRenderers::Color4Normal3::GetRenderer();
  color4TexCoord2Renderer = &DefaultRenderers::Color4TexCoord2::GetRenderer();
  color4Normal3TexCoord2Renderer =
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
      color4Renderer->DrawModelTransformed(
        modelID, transform, ResourceGroupHelper::GetEmptyResource());
      break;
    }
    case eVertexLayout::POSITION3_COLOR4_NORMAL3: {
      color4Normal3Renderer->DrawModelTransformed(
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
      return color4Renderer->AddRetainedModel(
        modelID, transform, ResourceGroupHelper::GetEmptyResource());
    }
    case eVertexLayout::POSITION3_COLOR4_NORMAL3: {
      return color4Normal3Renderer->AddRetainedModel(
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

  normal3Renderer->DrawModelTransformed(
    modelID, transform, ResourceGroupHelper::GetResourceGroup(materialID));
}

// ----------------------------------------------------------------------------

ModelInstanceID
CHIMIA_DRAW3D_NAMESPACE_NAME::AddRetainedModel(const ModelID& modelID,
                                               const glm::mat4x4& transform,
                                               const MaterialID& materialID)
{
  assert(ModelLayout(modelID) == eVertexLayout::POSITION3_NORMAL3);

  return normal3Renderer->AddRetainedModel(
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
      texCoord2Renderer->DrawModelTransformed(
        modelID, transform, ResourceGroupHelper::GetResourceGroup(textureID));
      break;
    }
    case eVertexLayout::POSITION3_NORMAL3_TEXCOORD2: {
      normal3TexCoord2Renderer->DrawModelTransformed(
        modelID, transform, ResourceGroupHelper::GetResourceGroup(textureID));
      break;
    }
    case eVertexLayout::POSITION3_COLOR4_TEXCOORD2: {
      color4TexCoord2Renderer->DrawModelTransformed(
        modelID, transform, ResourceGroupHelper::GetResourceGroup(textureID));
      break;
    }
    case eVertexLayout::POSITION3_COLOR4_NORMAL3_TEXCOORD2: {
      color4Normal3TexCoord2Renderer->DrawModelTransformed(
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
      return texCoord2Renderer->AddRetainedModel(
        modelID, transform, ResourceGroupHelper::GetResourceGroup(textureID));
    }
    case eVertexLayout::POSITION3_NORMAL3_TEXCOORD2: {
      return normal3TexCoord2Renderer->AddRetainedModel(
        modelID, transform, ResourceGroupHelper::GetResourceGroup(textureID));
    }
    case eVertexLayout::POSITION3_COLOR4_TEXCOORD2: {
      return color4TexCoord2Renderer->AddRetainedModel(
        modelID, transform, ResourceGroupHelper::GetResourceGroup(textureID));
    }
    case eVertexLayout::POSITION3_COLOR4_NORMAL3_TEXCOORD2: {
      return color4Normal3TexCoord2Renderer->AddRetainedModel(
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
CHIMIA_DRAW3D_NAMESPACE_NAME::DrawModel(const ModelID& modelID,
                                        const glm::mat4x4& transform,
                                        const ResourceGroupID& resource)
{
  const bool hasTexture = ResourceGroupHelper::HasTexture(resource);
  const bool hasMaterial = ResourceGroupHelper::HasMaterial(resource);

  const eVertexLayout layout = ModelLayout(modelID);
  switch (layout) {
    case eVertexLayout::POSITION3_COLOR4: {
      color4Renderer->DrawModelTransformed(modelID, transform, resource);
      break;
    }
    case eVertexLayout::POSITION3_NORMAL3: {
      assert(hasMaterial &&
             "DrawModel: Missing material for model with PN layout");
      normal3Renderer->DrawModelTransformed(modelID, transform, resource);
      break;
    }
    case eVertexLayout::POSITION3_TEXCOORD2: {
      assert(hasTexture &&
             "DrawModel: Missing texture for model with PT layout");
      texCoord2Renderer->DrawModelTransformed(modelID, transform, resource);
      break;
    }
    case eVertexLayout::POSITION3_COLOR4_NORMAL3: {
      color4Normal3Renderer->DrawModelTransformed(modelID, transform, resource);
      break;
    }
    case eVertexLayout::POSITION3_COLOR4_TEXCOORD2: {
      assert(hasTexture &&
             "DrawModel: Missing texture for model with PCT layout");
      color4TexCoord2Renderer->DrawModelTransformed(
        modelID, transform, resource);
      break;
    }
    case eVertexLayout::POSITION3_NORMAL3_TEXCOORD2: {
      assert(hasTexture &&
             "DrawModel: Missing texture for model with PNT layout");
      normal3TexCoord2Renderer->DrawModelTransformed(
        modelID, transform, resource);
      break;
    }
    case eVertexLayout::POSITION3_COLOR4_NORMAL3_TEXCOORD2: {
      assert(hasTexture &&
             "DrawModel: Missing texture for model with PCNT layout");
      color4Normal3TexCoord2Renderer->DrawModelTransformed(
        modelID, transform, resource);
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
                                               const ResourceGroupID& resource)
{
  const bool hasTexture = ResourceGroupHelper::HasTexture(resource);
  const bool hasMaterial = ResourceGroupHelper::HasMaterial(resource);

  const eVertexLayout layout = ModelLayout(modelID);
  switch (layout) {
    case eVertexLayout::POSITION3_COLOR4: {
      return color4Renderer->AddRetainedModel(modelID, transform, resource);
    }
    case eVertexLayout::POSITION3_NORMAL3: {
      assert(hasMaterial &&
             "AddRetainedModel: Missing material for model with PN layout");
      return normal3Renderer->AddRetainedModel(modelID, transform, resource);
    }
    case eVertexLayout::POSITION3_TEXCOORD2: {
      assert(hasTexture &&
             "AddRetainedModel: Missing texture for model with PT layout");
      return texCoord2Renderer->AddRetainedModel(modelID, transform, resource);
    }
    case eVertexLayout::POSITION3_COLOR4_NORMAL3: {
      return color4Normal3Renderer->AddRetainedModel(
        modelID, transform, resource);
    }
    case eVertexLayout::POSITION3_COLOR4_TEXCOORD2: {
      assert(hasTexture &&
             "AddRetainedModel: Missing texture for model with PCT layout");
      return color4TexCoord2Renderer->AddRetainedModel(
        modelID, transform, resource);
    }
    case eVertexLayout::POSITION3_NORMAL3_TEXCOORD2: {
      assert(hasTexture &&
             "AddRetainedModel: Missing texture for model with PNT layout");
      return normal3TexCoord2Renderer->AddRetainedModel(
        modelID, transform, resource);
    }
    case eVertexLayout::POSITION3_COLOR4_NORMAL3_TEXCOORD2: {
      assert(hasTexture &&
             "AddRetainedModel: Missing texture for model with PCNT layout");
      return color4Normal3TexCoord2Renderer->AddRetainedModel(
        modelID, transform, resource);
    }
    default:
      assert(false && "Unsuported model layout");
      return Draw3DPrivate::CreateModelInstanceID(0, 0, 0, 0);
  }
}

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

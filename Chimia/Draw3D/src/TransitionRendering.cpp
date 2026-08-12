#include "TransitionRendering.h"
#include "TransitionRenderingPrivate.h"

#include "GenericRenderer.h"
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
TransitionLayout(const TransitionID& transition)
{
  return ResourcesManager::GetInstance()
    .GetTransition(transition)
    ->GetVertexLayout();
}

}

// ----------------------------------------------------------------------------
// ModelRenderingPrivate
// ----------------------------------------------------------------------------

void
TransitionRenderingPrivate::Init()
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
CHIMIA_DRAW3D_NAMESPACE_NAME::DrawTransition(const TransitionID& transition,
                                             const float interpolation,
                                             const glm::mat4x4& transform)
{
  const eVertexLayout layout = TransitionLayout(transition);
  switch (layout) {
    case eVertexLayout::POSITION3_COLOR4: {
      color4Renderer->DrawTransition(transition,
                                     interpolation,
                                     transform,
                                     ResourceGroupHelper::GetEmptyResource());
      break;
    }
    case eVertexLayout::POSITION3_COLOR4_NORMAL3: {
      color4Normal3Renderer->DrawTransition(
        transition,
        interpolation,
        transform,
        ResourceGroupHelper::GetEmptyResource());
      break;
    }
    default:
      assert(false && "Unsuported transition layout");
  }
}

// ----------------------------------------------------------------------------
// Position3 + Normal3
// ----------------------------------------------------------------------------

void
CHIMIA_DRAW3D_NAMESPACE_NAME::DrawTransition(const TransitionID& transition,
                                             const float interpolation,
                                             const glm::mat4x4& transform,
                                             const MaterialID& materialID)
{
  assert(TransitionLayout(transition) == eVertexLayout::POSITION3_NORMAL3 &&
         "Unsuported transition layout");

  normal3Renderer->DrawTransition(
    transition,
    interpolation,
    transform,
    ResourceGroupHelper::GetResourceGroup(materialID));
}

// ----------------------------------------------------------------------------
// Position3 + TexCoord2
// Position3 + Color4 + TexCoord2
// Position3 + Normal3 + TexCoord2
// Position3 + Color4 + Normal3 + TexCoord2
// ----------------------------------------------------------------------------

void
CHIMIA_DRAW3D_NAMESPACE_NAME::DrawTransition(const TransitionID& transition,
                                             const float interpolation,
                                             const glm::mat4x4& transform,
                                             const TextureID& textureID)
{
  const eVertexLayout layout = TransitionLayout(transition);
  switch (layout) {
    case eVertexLayout::POSITION3_TEXCOORD2: {
      texCoord2Renderer->DrawTransition(
        transition,
        interpolation,
        transform,
        ResourceGroupHelper::GetResourceGroup(textureID));
      break;
    }
    case eVertexLayout::POSITION3_NORMAL3_TEXCOORD2: {
      normal3TexCoord2Renderer->DrawTransition(
        transition,
        interpolation,
        transform,
        ResourceGroupHelper::GetResourceGroup(textureID));
      break;
    }
    case eVertexLayout::POSITION3_COLOR4_TEXCOORD2: {
      color4TexCoord2Renderer->DrawTransition(
        transition,
        interpolation,
        transform,
        ResourceGroupHelper::GetResourceGroup(textureID));
      break;
    }
    case eVertexLayout::POSITION3_COLOR4_NORMAL3_TEXCOORD2: {
      color4Normal3TexCoord2Renderer->DrawTransition(
        transition,
        interpolation,
        transform,
        ResourceGroupHelper::GetResourceGroup(textureID));
      break;
    }
    default:
      assert(false && "Unsuported transition layout");
  }
}

// ----------------------------------------------------------------------------
// General
// ----------------------------------------------------------------------------

void
CHIMIA_DRAW3D_NAMESPACE_NAME::DrawTransition(const TransitionID& transition,
                                             const float interpolation,
                                             const glm::mat4x4& transform,
                                             const ResourceGroupID& resource)
{
  const bool hasTexture = ResourceGroupHelper::HasTexture(resource);
  const bool hasMaterial = ResourceGroupHelper::HasMaterial(resource);

  const eVertexLayout layout = TransitionLayout(transition);
  switch (layout) {
    case eVertexLayout::POSITION3_COLOR4: {
      color4Renderer->DrawTransition(
        transition, interpolation, transform, resource);
      break;
    }
    case eVertexLayout::POSITION3_NORMAL3: {
      assert(hasMaterial &&
             "DrawTransition: Missing material for model with PN layout");
      normal3Renderer->DrawTransition(
        transition, interpolation, transform, resource);
      break;
    }
    case eVertexLayout::POSITION3_TEXCOORD2: {
      assert(hasTexture &&
             "DrawTransition: Missing texture for model with PT layout");
      texCoord2Renderer->DrawTransition(
        transition, interpolation, transform, resource);
      break;
    }
    case eVertexLayout::POSITION3_COLOR4_NORMAL3: {
      color4Normal3Renderer->DrawTransition(
        transition, interpolation, transform, resource);
      break;
    }
    case eVertexLayout::POSITION3_COLOR4_TEXCOORD2: {
      assert(hasTexture &&
             "DrawTransition: Missing texture for model with PCT layout");
      color4TexCoord2Renderer->DrawTransition(
        transition, interpolation, transform, resource);
      break;
    }
    case eVertexLayout::POSITION3_NORMAL3_TEXCOORD2: {
      assert(hasTexture &&
             "DrawTransition: Missing texture for model with PNT layout");
      normal3TexCoord2Renderer->DrawTransition(
        transition, interpolation, transform, resource);
      break;
    }
    case eVertexLayout::POSITION3_COLOR4_NORMAL3_TEXCOORD2: {
      assert(hasTexture &&
             "DrawTransition: Missing texture for model with PCNT layout");
      color4Normal3TexCoord2Renderer->DrawTransition(
        transition, interpolation, transform, resource);
      break;
    }
    default:
      assert(false && "Unsuported transition layout");
  }
}

// ----------------------------------------------------------------------------

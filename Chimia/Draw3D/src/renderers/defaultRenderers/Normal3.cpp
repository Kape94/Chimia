#include "Normal3.h"

#include "DefaultRenderersNamespaceDefs.h"
#include "GenericRenderer.h"
#include "IlluminationPrivate.h"
#include "Renderers.h"
#include "RenderersUtils.h"
#include "ResourceGroup.h"
#include "ResourcesManager.h"
#include "Shaders.h"

#include "Rendering/Shader.h"
#include "Types.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE
USING_DEFAULT_RENDERERS_NAMESPACE

// ----------------------------------------------------------------------------

namespace {

constexpr eVertexLayout VERTEX_LAYOUT = eVertexLayout::POSITION3_NORMAL3;

void
ConfigureShaderForTriangleDrawing(const ResourcesGroup& resource)
{
  const MaterialID materialID = resource.FirstMaterial();
  auto material = ResourcesManager::GetInstance().GetMaterial(materialID);
  if (material == nullptr) {
    return;
  }

  Chimia::Rendering::Shader& shader = Shaders::Generic();
  shader.Use();

  RenderersUtils::ConfigureShaderForRendering(shader, VERTEX_LAYOUT, resource);
  IlluminationPrivate::ConfigureLightsOnShader(shader);
  IlluminationPrivate::ConfigureMaterialOnShader(*material, shader);
}

// ----------------------------------------------------------------------------

void
ConfigureShaderForTransformedModelDrawing(const ResourcesGroup& resource)
{
  const MaterialID materialID = resource.FirstMaterial();
  auto material = ResourcesManager::GetInstance().GetMaterial(materialID);
  if (material == nullptr) {
    return;
  }

  Chimia::Rendering::Shader& shader = Shaders::Generic();
  shader.Use();

  RenderersUtils::ConfigureShaderForInstancedRendering(
    shader, VERTEX_LAYOUT, resource);
  IlluminationPrivate::ConfigureLightsOnShader(shader);
  IlluminationPrivate::ConfigureMaterialOnShader(*material, shader);
}

GenericRenderer* g_renderer = nullptr;
}

// ----------------------------------------------------------------------------

void
Normal3::Init()
{
  g_renderer =
    &Renderers::CreateRenderer(VERTEX_LAYOUT,
                               ConfigureShaderForTriangleDrawing,
                               ConfigureShaderForTransformedModelDrawing);
}

// ----------------------------------------------------------------------------

GenericRenderer&
Normal3::GetRenderer()
{
  return *g_renderer;
}

// ----------------------------------------------------------------------------

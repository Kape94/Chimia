#include "Color4TexCoord2.h"

#include "DefaultRenderersNamespaceDefs.h"
#include "GenericRenderer.h"
#include "Renderers.h"
#include "RenderersUtils.h"
#include "ResourceGroup.h"
#include "Shaders.h"

#include "Rendering/Shader.h"
#include "Types.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE
USING_DEFAULT_RENDERERS_NAMESPACE

// ----------------------------------------------------------------------------

namespace {
constexpr eVertexLayout VERTEX_LAYOUT =
  eVertexLayout::POSITION3_COLOR4_TEXCOORD2;

void
ConfigureShaderForTriangleDrawing(const ResourcesGroup& resources)
{
  Chimia::Rendering::ShaderInstance& shader = Shaders::Generic();

  RenderersUtils::ConfigureShaderForRendering(shader, VERTEX_LAYOUT, resources);
}

void
ConfigureShaderForTransformedModelDrawing(const ResourcesGroup& resources)
{
  Chimia::Rendering::ShaderInstance& shader = Shaders::Generic();

  RenderersUtils::ConfigureShaderForInstancedRendering(
    shader, VERTEX_LAYOUT, resources);
}

void
ConfigureForTransitionDrawing(const ResourcesGroup& resource)
{
  Chimia::Rendering::ShaderInstance& shader = Shaders::Generic();

  RenderersUtils::ConfigureShaderForInstancedRendering(
    shader, VERTEX_LAYOUT, resource);
  RenderersUtils::ConfigureForTransitionRendering(shader);
}

GenericRenderer* g_renderer = nullptr;
}

// ----------------------------------------------------------------------------

void
Color4TexCoord2::Init()
{
  g_renderer =
    &Renderers::CreateRenderer(VERTEX_LAYOUT,
                               RenderersUtils::GetDefaultRenderingTarget(),
                               ConfigureShaderForTriangleDrawing,
                               ConfigureShaderForTransformedModelDrawing,
                               ConfigureForTransitionDrawing);
}

// ----------------------------------------------------------------------------

void
Color4TexCoord2::Shutdown()
{
  Renderers::DeleteRenderer(g_renderer);
}

// ----------------------------------------------------------------------------

GenericRenderer&
Color4TexCoord2::GetRenderer()
{
  return *g_renderer;
}

// ----------------------------------------------------------------------------

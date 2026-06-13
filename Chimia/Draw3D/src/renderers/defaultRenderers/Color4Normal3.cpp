#include "Color4Normal3.h"

#include "DefaultRenderersNamespaceDefs.h"
#include "GenericRenderer.h"
#include "IlluminationPrivate.h"
#include "Renderers.h"
#include "RenderersUtils.h"
#include "Shaders.h"

#include "Rendering/Shader.h"
#include "Types.h"
#include <glm/ext/vector_float3.hpp>

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE
USING_DEFAULT_RENDERERS_NAMESPACE

// ----------------------------------------------------------------------------

namespace {
constexpr eVertexLayout VERTEX_LAYOUT = eVertexLayout::POSITION3_COLOR4_NORMAL3;

void
ConfigureShaderForTriangleDrawing(const ResourcesGroup& resources)
{
  Chimia::Rendering::Shader& shader = Shaders::Generic();
  shader.Use();

  RenderersUtils::ConfigureShaderForRendering(shader, VERTEX_LAYOUT, resources);
  IlluminationPrivate::ConfigureLightsOnShader(shader);
}

void
ConfigureShaderForTransformedModelDrawing(const ResourcesGroup& resources)
{
  Chimia::Rendering::Shader& shader = Shaders::Generic();
  shader.Use();

  RenderersUtils::ConfigureShaderForInstancedRendering(
    shader, VERTEX_LAYOUT, resources);
  IlluminationPrivate::ConfigureLightsOnShader(shader);
}

GenericRenderer* g_renderer = nullptr;
}

// ----------------------------------------------------------------------------

void
Color4Normal3::Init()
{
  g_renderer =
    &Renderers::CreateRenderer(VERTEX_LAYOUT,
                               ConfigureShaderForTriangleDrawing,
                               ConfigureShaderForTransformedModelDrawing);
}

// ----------------------------------------------------------------------------

void
Color4Normal3::Shutdown()
{
  Renderers::DeleteRenderer(g_renderer);
}

// ----------------------------------------------------------------------------

GenericRenderer&
Color4Normal3::GetRenderer()
{
  return *g_renderer;
}

// ----------------------------------------------------------------------------

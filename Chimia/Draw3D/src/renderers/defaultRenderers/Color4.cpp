#include "Color4.h"

#include "CameraPrivate.h"
#include "DefaultRenderersNamespaceDefs.h"
#include "GenericRenderer.h"
#include "Renderers.h"
#include "RenderersUtils.h"
#include "ResourceGroup.h"
#include "Shaders.h"
#include "Types.h"

#include "Rendering/Shader.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE
USING_DEFAULT_RENDERERS_NAMESPACE

// ----------------------------------------------------------------------------

namespace {

constexpr eVertexLayout VERTEX_LAYOUT = eVertexLayout::POSITION3_COLOR4;

void
ConfigureForTriangleDrawing(const ResourcesGroup& resource)
{
  Chimia::Rendering::Shader& shader = Shaders::Generic();
  shader.Use();

  RenderersUtils::ConfigureShaderForRendering(shader, VERTEX_LAYOUT, resource);
  CameraPrivate::SetCameraOnShader(shader);
}

void
ConfigureForInstancedDrawing(const ResourcesGroup& resource)
{
  Chimia::Rendering::Shader& shader = Shaders::Generic();
  shader.Use();

  RenderersUtils::ConfigureShaderForInstancedRendering(
    shader, VERTEX_LAYOUT, resource);

  CameraPrivate::SetCameraOnShader(shader);
}

GenericRenderer* g_renderer = nullptr;
}

// ----------------------------------------------------------------------------

void
Color4::Init()
{
  g_renderer = &Renderers::CreateRenderer(
    VERTEX_LAYOUT, ConfigureForTriangleDrawing, ConfigureForInstancedDrawing);
}

// ----------------------------------------------------------------------------

GenericRenderer&
Color4::GetRenderer()
{
  return *g_renderer;
}

// ----------------------------------------------------------------------------

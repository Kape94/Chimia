#include "Color3.h"

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
Color3::Init()
{
  g_renderer = &Renderers::CreateRenderer(
    VERTEX_LAYOUT, ConfigureForTriangleDrawing, ConfigureForInstancedDrawing);
}

// ----------------------------------------------------------------------------

GenericRenderer&
Color3::GetRenderer()
{
  return *g_renderer;
}

// ----------------------------------------------------------------------------

void
Color3::DrawTriangle(const glm::vec3& p1,
                     const glm::vec4& color1,
                     const glm::vec3& p2,
                     const glm::vec4& color2,
                     const glm::vec3& p3,
                     const glm::vec4& color3,
                     const ResourceGroupID& resource)
{
  constexpr size_t POS3_SIZE = sizeof(glm::vec3);
  constexpr size_t COL4_SIZE = sizeof(glm::vec4);

  auto& renderer = GetRenderer();
  renderer.DrawTriangle({ { &p1, POS3_SIZE },
                          { &color1, COL4_SIZE },
                          { &p2, POS3_SIZE },
                          { &color2, COL4_SIZE },
                          { &p3, POS3_SIZE },
                          { &color3, COL4_SIZE } },
                        resource);
}

// ----------------------------------------------------------------------------

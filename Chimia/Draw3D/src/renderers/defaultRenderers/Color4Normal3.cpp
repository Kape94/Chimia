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

GenericRenderer&
Color4Normal3::GetRenderer()
{
  return *g_renderer;
}

// ----------------------------------------------------------------------------

void
Color4Normal3::DrawTriangle(const glm::vec3& p1,
                            const glm::vec4& p1Color,
                            const glm::vec3& p1Normal,
                            const glm::vec3& p2,
                            const glm::vec4& p2Color,
                            const glm::vec3& p2Normal,
                            const glm::vec3& p3,
                            const glm::vec4& p3Color,
                            const glm::vec3& p3Normal,
                            const ResourceGroupID& resource)
{
  constexpr size_t POS3_SIZE = sizeof(glm::vec3);
  constexpr size_t COLOR4_SIZE = sizeof(glm::vec4);
  constexpr size_t NORM3_SIZE = sizeof(glm::vec3);

  auto& renderer = GetRenderer();
  renderer.DrawTriangle(
    {
      { &p1, POS3_SIZE },
      { &p1Color, COLOR4_SIZE },
      { &p1Normal, NORM3_SIZE },
      { &p2, POS3_SIZE },
      { &p2Color, COLOR4_SIZE },
      { &p2Normal, NORM3_SIZE },
      { &p3, POS3_SIZE },
      { &p3Color, COLOR4_SIZE },
      { &p3Normal, NORM3_SIZE },
    },
    resource);
}

// ----------------------------------------------------------------------------

#include "Color3Normal3.h"

#include "Config.h"
#include "DefaultRenderersNamespaceDefs.h"
#include "GenericRenderer.h"
#include "IlluminationPrivate.h"
#include "Renderers.h"
#include "Shaders.h"

#include "Rendering/Shader.h"
#include "Types.h"
#include <glm/ext/vector_float3.hpp>

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE
USING_DEFAULT_RENDERERS_NAMESPACE

// ----------------------------------------------------------------------------

namespace {

void
ConfigureShaderForTriangleDrawing(const ResourcesGroup&)
{
  Chimia::Rendering::Shader& shader = Shaders::Generic();
  shader.Use();

  shader.SetUniform("hasVertexColor", true);
  shader.SetUniform("hasNormal", true);
  shader.SetUniform("hasTexCoord", false);
  shader.SetUniform("isInstanced", false);
  shader.SetUniform("hasMaterial", false);
  shader.SetUniform("hasTexture", false);

  const int illuminationModel = static_cast<int>(Config::IlluminationModel());
  shader.SetUniform("lightningModel", illuminationModel);

  IlluminationPrivate::ConfigureLightsOnShader(shader);
}

void
ConfigureShaderForTransformedModelDrawing(const ResourcesGroup&)
{
  Chimia::Rendering::Shader& shader = Shaders::Generic();
  shader.Use();

  shader.SetUniform("hasVertexColor", true);
  shader.SetUniform("hasNormal", true);
  shader.SetUniform("hasTexCoord", false);
  shader.SetUniform("isInstanced", true);
  shader.SetUniform("hasMaterial", false);
  shader.SetUniform("hasTexture", false);

  const int illuminationModel = static_cast<int>(Config::IlluminationModel());
  shader.SetUniform("lightningModel", illuminationModel);

  IlluminationPrivate::ConfigureLightsOnShader(shader);
}

GenericRenderer* g_renderer = nullptr;
}

// ----------------------------------------------------------------------------

void
Color3Normal3::Init()
{
  g_renderer =
    &Renderers::CreateRenderer(eVertexLayout::POSITION3_COLOR3_NORMAL3,
                               ConfigureShaderForTriangleDrawing,
                               ConfigureShaderForTransformedModelDrawing);
}

// ----------------------------------------------------------------------------

GenericRenderer&
Color3Normal3::GetRenderer()
{
  return *g_renderer;
}

// ----------------------------------------------------------------------------

void
Color3Normal3::DrawTriangle(const glm::vec3& p1,
                            const glm::vec3& p1Color,
                            const glm::vec3& p1Normal,
                            const glm::vec3& p2,
                            const glm::vec3& p2Color,
                            const glm::vec3& p2Normal,
                            const glm::vec3& p3,
                            const glm::vec3& p3Color,
                            const glm::vec3& p3Normal,
                            const ResourceGroupID& resource)
{
  constexpr size_t POS3_SIZE = sizeof(glm::vec3);
  constexpr size_t COLOR3_SIZE = sizeof(glm::vec3);
  constexpr size_t NORM3_SIZE = sizeof(glm::vec3);

  auto& renderer = GetRenderer();
  renderer.DrawTriangle(
    {
      { &p1, POS3_SIZE },
      { &p1Color, COLOR3_SIZE },
      { &p1Normal, NORM3_SIZE },
      { &p2, POS3_SIZE },
      { &p2Color, COLOR3_SIZE },
      { &p2Normal, NORM3_SIZE },
      { &p3, POS3_SIZE },
      { &p3Color, COLOR3_SIZE },
      { &p3Normal, NORM3_SIZE },
    },
    resource);
}

// ----------------------------------------------------------------------------

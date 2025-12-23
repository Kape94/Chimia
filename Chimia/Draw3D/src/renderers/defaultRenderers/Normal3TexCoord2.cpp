#include "Normal3TexCoord2.h"

#include "DefaultRenderersNamespaceDefs.h"
#include "GenericRenderer.h"
#include "IlluminationPrivate.h"
#include "Renderers.h"
#include "RenderersUtils.h"
#include "ResourceGroup.h"
#include "ResourcesManager.h"
#include "Shaders.h"

#include "Rendering/Shader.h"
#include "Rendering/TextureUnit.h"
#include "Types.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE
USING_DEFAULT_RENDERERS_NAMESPACE

// ----------------------------------------------------------------------------

namespace {

constexpr eVertexLayout VERTEX_LAYOUT =
  eVertexLayout::POSITION3_NORMAL3_TEXCOORD2;

void
ConfigureShaderForTriangleDrawing(const ResourcesGroup& resource)
{
  const TextureID textureID = resource.FirstTexture();
  auto texture = ResourcesManager::GetInstance().GetTexture(textureID);
  if (texture == nullptr) {
    return;
  }

  Chimia::Rendering::Shader& shader = Shaders::Generic();
  shader.Use();

  RenderersUtils::ConfigureShaderForRendering(shader, VERTEX_LAYOUT, resource);
  IlluminationPrivate::ConfigureLightsOnShader(shader);

  constexpr auto TEXTURE_UNIT = Chimia::Rendering::TextureUnit::UNIT_1;

  texture->Use(TEXTURE_UNIT);
  shader.SetUniform("tex", TEXTURE_UNIT);
}

// ----------------------------------------------------------------------------

void
ConfigureShaderForTransformedModelDrawing(const ResourcesGroup& resource)
{
  const TextureID textureID = resource.FirstTexture();
  auto texture = ResourcesManager::GetInstance().GetTexture(textureID);
  if (texture == nullptr) {
    return;
  }

  Chimia::Rendering::Shader& shader = Shaders::Generic();
  shader.Use();

  RenderersUtils::ConfigureShaderForInstancedRendering(
    shader, VERTEX_LAYOUT, resource);
  IlluminationPrivate::ConfigureLightsOnShader(shader);

  constexpr auto TEXTURE_UNIT = Chimia::Rendering::TextureUnit::UNIT_1;

  texture->Use(TEXTURE_UNIT);
  shader.SetUniform("tex", TEXTURE_UNIT);
}

GenericRenderer* g_renderer = nullptr;
}

// ----------------------------------------------------------------------------

void
Normal3TexCoord2::Init()
{
  g_renderer =
    &Renderers::CreateRenderer(VERTEX_LAYOUT,
                               ConfigureShaderForTriangleDrawing,
                               ConfigureShaderForTransformedModelDrawing);
}

// ----------------------------------------------------------------------------

GenericRenderer&
Normal3TexCoord2::GetRenderer()
{
  return *g_renderer;
}

// ----------------------------------------------------------------------------

void
Normal3TexCoord2::DrawTriangle(const glm::vec3& p1,
                               const glm::vec3& p1Normal,
                               const glm::vec2& p1TexCoord,
                               const glm::vec3& p2,
                               const glm::vec3& p2Normal,
                               const glm::vec2& p2TexCoord,
                               const glm::vec3& p3,
                               const glm::vec3& p3Normal,
                               const glm::vec2& p3TexCoord,
                               const ResourceGroupID& resource)
{
  constexpr size_t POS3_SIZE = sizeof(glm::vec3);
  constexpr size_t TEX_COORD2_SIZE = sizeof(glm::vec2);
  constexpr size_t NORM3_SIZE = sizeof(glm::vec3);

  auto& renderer = GetRenderer();
  renderer.DrawTriangle(
    {
      { &p1, POS3_SIZE },
      { &p1Normal, NORM3_SIZE },
      { &p1TexCoord, TEX_COORD2_SIZE },
      { &p2, POS3_SIZE },
      { &p2Normal, NORM3_SIZE },
      { &p2TexCoord, TEX_COORD2_SIZE },
      { &p3, POS3_SIZE },
      { &p3Normal, NORM3_SIZE },
      { &p3TexCoord, TEX_COORD2_SIZE },
    },
    resource);
}

// ----------------------------------------------------------------------------

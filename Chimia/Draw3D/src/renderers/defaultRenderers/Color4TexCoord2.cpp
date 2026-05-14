#include "Color4TexCoord2.h"

#include "CameraPrivate.h"
#include "DefaultRenderersNamespaceDefs.h"
#include "GenericRenderer.h"
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
  eVertexLayout::POSITION3_COLOR4_TEXCOORD2;

void
ConfigureShaderForTriangleDrawing(const ResourcesGroup& resources)
{
  const TextureID textureID = resources.FirstTexture();
  auto texture = ResourcesManager::GetInstance().GetTexture(textureID);
  if (texture == nullptr) {
    return;
  }

  Chimia::Rendering::Shader& shader = Shaders::Generic();
  shader.Use();

  RenderersUtils::ConfigureShaderForRendering(shader, VERTEX_LAYOUT, resources);
  CameraPrivate::SetCameraOnShader(shader);

  constexpr auto TEXTURE_UNIT = Chimia::Rendering::TextureUnit::UNIT_1;

  texture->Use(TEXTURE_UNIT);
  shader.SetUniform("tex", TEXTURE_UNIT);
}

void
ConfigureShaderForTransformedModelDrawing(const ResourcesGroup& resources)
{
  const TextureID textureID = resources.FirstTexture();
  auto texture = ResourcesManager::GetInstance().GetTexture(textureID);
  if (texture == nullptr) {
    return;
  }

  Chimia::Rendering::Shader& shader = Shaders::Generic();
  shader.Use();

  RenderersUtils::ConfigureShaderForInstancedRendering(
    shader, VERTEX_LAYOUT, resources);
  CameraPrivate::SetCameraOnShader(shader);

  constexpr auto TEXTURE_UNIT = Chimia::Rendering::TextureUnit::UNIT_1;

  texture->Use(TEXTURE_UNIT);
  shader.SetUniform("tex", TEXTURE_UNIT);
}

GenericRenderer* g_renderer = nullptr;
}

// ----------------------------------------------------------------------------

void
Color4TexCoord2::Init()
{
  g_renderer =
    &Renderers::CreateRenderer(VERTEX_LAYOUT,
                               ConfigureShaderForTriangleDrawing,
                               ConfigureShaderForTransformedModelDrawing);
}

// ----------------------------------------------------------------------------

GenericRenderer&
Color4TexCoord2::GetRenderer()
{
  return *g_renderer;
}

// ----------------------------------------------------------------------------

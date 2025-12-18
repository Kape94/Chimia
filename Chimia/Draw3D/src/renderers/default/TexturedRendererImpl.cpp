#include "TexturedRendererImpl.h"

#include "CameraPrivate.h"
#include "GenericRenderer.h"
#include "Renderers.h"
#include "ResourcesManager.h"
#include "Shaders.h"

#include "Rendering/Shader.h"
#include "Rendering/TextureUnit.h"
#include "Types.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace {
void
ConfigureShaderForTriangleDrawing(const ResourcesGroup& resource)
{
  const TextureID textureID = resource.FirstTexture();
  auto texture = ResourcesManager::GetInstance().GetTexture(textureID);
  if (texture == nullptr) {
    return;
  }

  Chimia::Rendering::Shader& shader = Shaders::Textured();
  shader.Use();

  CameraPrivate::SetCameraOnShader(shader);

  constexpr auto TEXTURE_UNIT = Chimia::Rendering::TextureUnit::UNIT_1;

  texture->Use(TEXTURE_UNIT);
  shader.SetUniform("tex", TEXTURE_UNIT);
}

void
ConfigureShaderForTransformedModelDrawing(const ResourcesGroup& resource)
{
  const TextureID textureID = resource.FirstTexture();
  auto texture = ResourcesManager::GetInstance().GetTexture(textureID);
  if (texture == nullptr) {
    return;
  }

  Chimia::Rendering::Shader& shader = Shaders::TexturedWithInstancedTransform();
  shader.Use();

  CameraPrivate::SetCameraOnShader(shader);

  constexpr auto TEXTURE_UNIT = Chimia::Rendering::TextureUnit::UNIT_1;

  texture->Use(TEXTURE_UNIT);
  shader.SetUniform("tex", TEXTURE_UNIT);
}

GenericRenderer* g_renderer = nullptr;
}

// ----------------------------------------------------------------------------

void
TexturedRendererImpl::Init()
{
  g_renderer =
    &Renderers::CreateRenderer(eVertexLayout::POSITION3_TEXCOORD2,
                               ConfigureShaderForTriangleDrawing,
                               ConfigureShaderForTransformedModelDrawing);
}

// ----------------------------------------------------------------------------

GenericRenderer&
TexturedRendererImpl::GetRenderer()
{
  return *g_renderer;
}

// ----------------------------------------------------------------------------

void
TexturedRendererImpl::DrawTriangle(const glm::vec3& p1,
                                   const glm::vec2& p1TexCoord,
                                   const glm::vec3& p2,
                                   const glm::vec2& p2TexCoord,
                                   const glm::vec3& p3,
                                   const glm::vec2& p3TexCoord,
                                   const ResourceGroupID& resourceID)
{
  constexpr size_t POS3_SIZE = sizeof(glm::vec3);
  constexpr size_t TEX_COORD2_SIZE = sizeof(glm::vec2);

  static auto& renderer = GetRenderer();

  renderer.DrawTriangle(
    {
      { &p1, POS3_SIZE },
      { &p1TexCoord, TEX_COORD2_SIZE },
      { &p2, POS3_SIZE },
      { &p2TexCoord, TEX_COORD2_SIZE },
      { &p3, POS3_SIZE },
      { &p3TexCoord, TEX_COORD2_SIZE },
    },
    resourceID);
}

// ----------------------------------------------------------------------------

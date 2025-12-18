#include "ColoredTexturedRendererImpl.h"

#include "CameraPrivate.h"
#include "GenericRenderer.h"
#include "Renderers.h"
#include "ResourceGroup.h"
#include "ResourcesManager.h"
#include "Shaders.h"

#include "Rendering/Shader.h"
#include "Rendering/TextureUnit.h"
#include "Types.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace {

Chimia::Rendering::Shader&
GetShaderForTriangleMeshDrawing()
{
  return Chimia::Draw3D::Shaders::ColoredTextured();
}

Chimia::Rendering::Shader&
GetShaderForModelDrawing()
{
  return Chimia::Draw3D::Shaders::ColoredTexturedWithInstancedTransform();
}

void
ConfigureShaderForTriangleDrawing(const ResourcesGroup& resources)
{
  const TextureID textureID = resources.FirstTexture();
  auto texture = ResourcesManager::GetInstance().GetTexture(textureID);
  if (texture == nullptr) {
    return;
  }

  Chimia::Rendering::Shader& shader = GetShaderForTriangleMeshDrawing();
  shader.Use();

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

  Chimia::Rendering::Shader& shader = GetShaderForModelDrawing();
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
ColoredTexturedRendererImpl::Init()
{
  g_renderer =
    &Renderers::CreateRenderer(eVertexLayout::POSITION3_COLOR3_TEXCOORD2,
                               ConfigureShaderForTriangleDrawing,
                               ConfigureShaderForTransformedModelDrawing);
}

// ----------------------------------------------------------------------------

GenericRenderer&
ColoredTexturedRendererImpl::GetRenderer()
{
  return *g_renderer;
}

// ----------------------------------------------------------------------------

void
ColoredTexturedRendererImpl::DrawTriangle(const glm::vec3& p1,
                                          const glm::vec3& p1Color,
                                          const glm::vec2& p1TexCoord,
                                          const glm::vec3& p2,
                                          const glm::vec3& p2Color,
                                          const glm::vec2& p2TexCoord,
                                          const glm::vec3& p3,
                                          const glm::vec3& p3Color,
                                          const glm::vec2& p3TexCoord,
                                          const ResourceGroupID& resource)
{
  constexpr size_t POS3_SIZE = sizeof(glm::vec3);
  constexpr size_t COLOR3_SIZE = sizeof(glm::vec3);
  constexpr size_t TEX_COORD2_SIZE = sizeof(glm::vec2);

  auto& renderer = GetRenderer();
  renderer.DrawTriangle(
    {
      { &p1, POS3_SIZE },
      { &p1Color, COLOR3_SIZE },
      { &p1TexCoord, TEX_COORD2_SIZE },
      { &p2, POS3_SIZE },
      { &p2Color, COLOR3_SIZE },
      { &p2TexCoord, TEX_COORD2_SIZE },
      { &p3, POS3_SIZE },
      { &p3Color, COLOR3_SIZE },
      { &p3TexCoord, TEX_COORD2_SIZE },
    },
    resource);
}

// ----------------------------------------------------------------------------

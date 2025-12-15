#include "ColoredTexturedRendererImpl.h"

#include "CameraPrivate.h"
#include "GenericRenderer.h"
#include "ResourceGroup.h"
#include "ResourcesManager.h"
#include "Shaders.h"

#include "Rendering/Shader.h"
#include "Rendering/ShaderAttribute.h"
#include "Rendering/TextureUnit.h"
#include "Types.h"
#include "eRendererType.h"

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
}

// ----------------------------------------------------------------------------

GenericRenderer&
ColoredTexturedRendererImpl::GetRenderer()
{
  constexpr unsigned RENDERER_ID =
    static_cast<unsigned>(eRendererType::COLORED_TEXTURED);

  static const Chimia::Rendering::ShaderAttributes VERTEX_ATTRIBUTES{
    Chimia::Rendering::ShaderAttribute::Float(0 /*pos*/, 3),
    Chimia::Rendering::ShaderAttribute::Float(1 /*color*/, 3),
    Chimia::Rendering::ShaderAttribute::Float(2 /*texCoord*/, 2)
  };

  static const Chimia::Rendering::ShaderAttributes
    TRANSFORMED_MODELS_INSTANCE_ATTRIBUTES{
      Chimia::Rendering::ShaderAttribute::Float(3 /*transform*/, 4),
      Chimia::Rendering::ShaderAttribute::Float(4 /*transform*/, 4),
      Chimia::Rendering::ShaderAttribute::Float(5 /*transform*/, 4),
      Chimia::Rendering::ShaderAttribute::Float(6 /*transform*/, 4)
    };

  static GenericRenderer renderer(RENDERER_ID,
                                  VERTEX_ATTRIBUTES,
                                  TRANSFORMED_MODELS_INSTANCE_ATTRIBUTES,
                                  ConfigureShaderForTriangleDrawing,
                                  ConfigureShaderForTransformedModelDrawing);
  return renderer;
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

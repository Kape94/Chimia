#include "TexturedRendererImpl.h"

#include "CameraPrivate.h"
#include "GenericRenderer.h"
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

}

// ----------------------------------------------------------------------------

GenericRenderer&
TexturedRendererImpl::GetRenderer()
{
  static const Chimia::Rendering::ShaderAttributes VERTEX_ATTRIBUTES{
    Chimia::Rendering::ShaderAttribute::Float(0 /*pos*/, 3),
    Chimia::Rendering::ShaderAttribute::Float(1 /*texCoord*/, 2)
  };

  static const Chimia::Rendering::ShaderAttributes
    TRANSFORMED_MODELS_INSTANCE_ATTRIBUTES{
      Chimia::Rendering::ShaderAttribute::Float(2 /*transform*/, 4),
      Chimia::Rendering::ShaderAttribute::Float(3 /*transform*/, 4),
      Chimia::Rendering::ShaderAttribute::Float(4 /*transform*/, 4),
      Chimia::Rendering::ShaderAttribute::Float(5 /*transform*/, 4)
    };

  constexpr unsigned RENDERER_ID =
    static_cast<unsigned>(eRendererType::TEXTURED);

  static GenericRenderer renderer(RENDERER_ID,
                                  VERTEX_ATTRIBUTES,
                                  TRANSFORMED_MODELS_INSTANCE_ATTRIBUTES,
                                  ConfigureShaderForTriangleDrawing,
                                  ConfigureShaderForTransformedModelDrawing);
  return renderer;
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

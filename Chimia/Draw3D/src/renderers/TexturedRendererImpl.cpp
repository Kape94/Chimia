#include "TexturedRendererImpl.h"

#include "CameraPrivate.h"
#include "Core/Types.h"
#include "ResourceGroupHelper.h"
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

TexturedRendererImpl&
TexturedRendererImpl::getInstance()
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

  static TexturedRendererImpl renderer(
    RENDERER_ID,
    VERTEX_ATTRIBUTES,
    TRANSFORMED_MODELS_INSTANCE_ATTRIBUTES,
    ConfigureShaderForTriangleDrawing,
    ConfigureShaderForTransformedModelDrawing);
  return renderer;
}

// ----------------------------------------------------------------------------

TexturedRendererImpl::TexturedRendererImpl(
  const unsigned id,
  const Rendering::ShaderAttributes& vertexAttributes,
  const Rendering::ShaderAttributes& instancedAttributes,
  void (*setupShaderForTriangleRendering)(const ResourcesGroup&),
  void (*setupShaderForInstancedRendering)(const ResourcesGroup&))
  : m_renderer(id,
               vertexAttributes,
               instancedAttributes,
               setupShaderForTriangleRendering,
               setupShaderForInstancedRendering)
{
}

// ----------------------------------------------------------------------------

void
TexturedRendererImpl::Init()
{
}

// ----------------------------------------------------------------------------

void
TexturedRendererImpl::DrawTriangle(const glm::vec3& p1,
                                   const glm::vec2& p1TexCoord,
                                   const glm::vec3& p2,
                                   const glm::vec2& p2TexCoord,
                                   const glm::vec3& p3,
                                   const glm::vec2& p3TexCoord,
                                   const TextureID& textureID)
{
  constexpr size_t POS3_SIZE = sizeof(glm::vec3);
  constexpr size_t TEX_COORD2_SIZE = sizeof(glm::vec2);

  m_renderer.DrawTriangle(
    {
      { &p1, POS3_SIZE },
      { &p1TexCoord, TEX_COORD2_SIZE },
      { &p2, POS3_SIZE },
      { &p2TexCoord, TEX_COORD2_SIZE },
      { &p3, POS3_SIZE },
      { &p3TexCoord, TEX_COORD2_SIZE },
    },
    ResourceGroupHelper::GetResourceGroup(textureID));
}

// ----------------------------------------------------------------------------

void
TexturedRendererImpl::DrawTriangles(const RawArrayView& vertexDataArray,
                                    const TextureID& textureID)
{
  m_renderer.DrawTriangles(vertexDataArray,
                           ResourceGroupHelper::GetResourceGroup(textureID));
}

// ----------------------------------------------------------------------------

TriangleMeshID
TexturedRendererImpl::AddStaticTriangles(const RawDataView& vertexData,
                                         const TextureID& textureID)
{
  return m_renderer.AddStaticTriangles(
    vertexData, ResourceGroupHelper::GetResourceGroup(textureID));
}

// ----------------------------------------------------------------------------

void
TexturedRendererImpl::DeleteStaticTriangles(const TriangleMeshID& meshID)
{
  m_renderer.DeleteStaticTriangles(meshID);
}

// ----------------------------------------------------------------------------

void
TexturedRendererImpl::DrawModelTransformed(const ModelID& modelID,
                                           const glm::mat4x4& transform,
                                           const TextureID& textureID)
{
  m_renderer.DrawModelTransformed(
    modelID, transform, ResourceGroupHelper::GetResourceGroup(textureID));
}

// ----------------------------------------------------------------------------

ModelInstanceID
TexturedRendererImpl::AddStaticModel(const ModelID& modelID,
                                     const glm::mat4x4& transform,
                                     const TextureID& textureID)
{
  return m_renderer.AddStaticModel(
    modelID, transform, ResourceGroupHelper::GetResourceGroup(textureID));
}

// ----------------------------------------------------------------------------

void
TexturedRendererImpl::DeleteStaticModel(const ModelInstanceID& instanceID)
{
  m_renderer.DeleteStaticModel(instanceID);
}

// ----------------------------------------------------------------------------

void
TexturedRendererImpl::Flush()
{
  m_renderer.Flush();
}

// ----------------------------------------------------------------------------

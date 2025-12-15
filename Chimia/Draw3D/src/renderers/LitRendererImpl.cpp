#include "LitRendererImpl.h"

#include "Config.h"
#include "GenericRenderer.h"
#include "IlluminationPrivate.h"
#include "ResourceGroup.h"
#include "ResourcesManager.h"
#include "Shaders.h"

#include "Rendering/Shader.h"
#include "Rendering/ShaderAttribute.h"
#include "Types.h"
#include "eRendererType.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace {

Chimia::Rendering::Shader&
GetShaderForTriangleMeshDrawing()
{
  return Chimia::Draw3D::Config::IlluminationModel() ==
             eIlluminationModel::GOURAUD
           ? Chimia::Draw3D::Shaders::GouraudLit()
           : Chimia::Draw3D::Shaders::PhongLit();
}

Chimia::Rendering::Shader&
GetShaderForModelDrawing()
{
  return Config::IlluminationModel() == eIlluminationModel::GOURAUD
           ? Chimia::Draw3D::Shaders::
               GouraudLitWithInstancedTransformAndMaterial()
           : Chimia::Draw3D::Shaders::
               PhongLitWithInstancedTransformAndMaterial();
}

void
ConfigureShaderForTriangleDrawing(const ResourcesGroup& resource)
{
  const MaterialID materialID = resource.FirstMaterial();
  auto material = ResourcesManager::GetInstance().GetMaterial(materialID);
  if (material == nullptr) {
    return;
  }

  Chimia::Rendering::Shader& shader = GetShaderForTriangleMeshDrawing();

  shader.Use();
  IlluminationPrivate::ConfigureLightsOnShader(shader);
  IlluminationPrivate::ConfigureMaterialOnShader(*material, shader);
}

// ----------------------------------------------------------------------------

void
ConfigureShaderForTransformedModelDrawing(const ResourcesGroup& resource)
{
  const MaterialID materialID = resource.FirstMaterial();
  auto material = ResourcesManager::GetInstance().GetMaterial(materialID);
  if (material == nullptr) {
    return;
  }

  Chimia::Rendering::Shader& shader = GetShaderForModelDrawing();

  shader.Use();
  IlluminationPrivate::ConfigureLightsOnShader(shader);
  IlluminationPrivate::ConfigureMaterialOnShader(*material, shader);
}
}

// ----------------------------------------------------------------------------

GenericRenderer&
LitRendererImpl::GetRenderer()
{
  constexpr unsigned RENDERER_ID = static_cast<unsigned>(eRendererType::LIT);

  static const Chimia::Rendering::ShaderAttributes VERTEX_ATTRIBUTES{
    Chimia::Rendering::ShaderAttribute::Float(0 /*pos*/, 3),
    Chimia::Rendering::ShaderAttribute::Float(1 /*normal*/, 3)
  };

  static const Chimia::Rendering::ShaderAttributes
    TRANSFORMED_MODELS_INSTANCE_ATTRIBUTES{
      Chimia::Rendering::ShaderAttribute::Float(2 /*transform*/, 4),
      Chimia::Rendering::ShaderAttribute::Float(3 /*transform*/, 4),
      Chimia::Rendering::ShaderAttribute::Float(4 /*transform*/, 4),
      Chimia::Rendering::ShaderAttribute::Float(5 /*transform*/, 4)
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
LitRendererImpl::DrawTriangle(const glm::vec3& p1,
                              const glm::vec3& p1Normal,
                              const glm::vec3& p2,
                              const glm::vec3& p2Normal,
                              const glm::vec3& p3,
                              const glm::vec3& p3Normal,
                              const ResourceGroupID& resource)
{
  constexpr size_t POS3_SIZE = sizeof(glm::vec3);
  constexpr size_t NORM3_SIZE = sizeof(glm::vec3);

  auto& renderer = GetRenderer();
  renderer.DrawTriangle(
    {
      { &p1, POS3_SIZE },
      { &p1Normal, NORM3_SIZE },
      { &p2, POS3_SIZE },
      { &p2Normal, NORM3_SIZE },
      { &p3, POS3_SIZE },
      { &p3Normal, NORM3_SIZE },
    },
    resource);
}

// ----------------------------------------------------------------------------

#include "RenderersUtils.h"

#include "CameraPrivate.h"
#include "Config.h"
#include "IlluminationPrivate.h"
#include "Pipelines.h"
#include "ResourceGroup.h"
#include "ResourcesManager.h"
#include "ShaderUniformsNames.h"
#include "Types.h"

#include "Core/Diagnostics.h"
#include "Rendering/Shader.h"
#include "Rendering/ShaderAttribute.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
// RenderersUtilsPrivate
// ----------------------------------------------------------------------------

namespace RenderersUtilsPrivate {
using namespace Chimia;

Rendering::ShaderAttribute
PositionAttribute()
{
  return Rendering::ShaderAttribute::Float(0 /*pos*/, 3);
}

Rendering::ShaderAttribute
ColorAttribute()
{
  return Rendering::ShaderAttribute::Float(1 /*color*/, 4);
}

Rendering::ShaderAttribute
NormalAttribute()
{
  return Rendering::ShaderAttribute::Float(2 /*normal*/, 3);
}

Rendering::ShaderAttribute
TexCoordAttribute()
{
  return Rendering::ShaderAttribute::Float(3 /*texCoord*/, 2);
}

Rendering::ShaderAttributes
TransformAttributes()
{
  return { Rendering::ShaderAttribute::Float(4 /*transform*/, 4),
           Rendering::ShaderAttribute::Float(5 /*transform*/, 4),
           Rendering::ShaderAttribute::Float(6 /*transform*/, 4),
           Rendering::ShaderAttribute::Float(7 /*transform*/, 4) };
}

bool
HasColor(const eVertexLayout& layout)
{
  return layout == eVertexLayout::POSITION3_COLOR4 ||
         layout == eVertexLayout::POSITION3_COLOR4_NORMAL3 ||
         layout == eVertexLayout::POSITION3_COLOR4_TEXCOORD2 ||
         layout == eVertexLayout::POSITION3_COLOR4_NORMAL3_TEXCOORD2;
}

bool
HasNormal(const eVertexLayout& layout)
{
  return layout == eVertexLayout::POSITION3_NORMAL3 ||
         layout == eVertexLayout::POSITION3_COLOR4_NORMAL3 ||
         layout == eVertexLayout::POSITION3_NORMAL3_TEXCOORD2 ||
         layout == eVertexLayout::POSITION3_COLOR4_NORMAL3_TEXCOORD2;
}

bool
HasTexCoord(const eVertexLayout& layout)
{
  return layout == eVertexLayout::POSITION3_TEXCOORD2 ||
         layout == eVertexLayout::POSITION3_COLOR4_TEXCOORD2 ||
         layout == eVertexLayout::POSITION3_NORMAL3_TEXCOORD2 ||
         layout == eVertexLayout::POSITION3_COLOR4_NORMAL3_TEXCOORD2;
}

void
ConfigureOpacity(Rendering::Shader& shader,
                 const eVertexLayout& layout,
                 const ResourcesGroup& resources)
{
  const bool allowsOpacity = !HasNormal(layout) && resources.HasOpacityFactor();

  if (allowsOpacity) {
    const OpacityFactorID opacityID = resources.FirstOpacityFactor();
    const float* opacity =
      ResourcesManager::GetInstance().GetOpacityFactor(opacityID);

    shader.SetUniform(ShaderUniformsNames::OPACITY, *opacity);
  } else {
    shader.SetUniform(ShaderUniformsNames::OPACITY, 1.0f);
  }
}

void
ConfigureMixtureColor(Rendering::Shader& shader,
                      const ResourcesGroup& resources)
{
  if (resources.HasMixtureColor()) {
    const MixtureColorID colorID = resources.FirstMixtureColor();
    const glm::vec3* color =
      ResourcesManager::GetInstance().GetMixtureColor(colorID);

    shader.SetUniform(ShaderUniformsNames::MIXTURE_COLOR, *color);

  } else {
    shader.SetUniform(ShaderUniformsNames::MIXTURE_COLOR,
                      glm::vec3(1.0f, 1.0f, 1.0f));
  }
}

void
ConfigureMaterialOnShader(const ResourcesGroup& resources,
                          Rendering::Shader& shader)
{
  if (!resources.HasMaterials()) {
    return;
  }

  const MaterialID materialID = resources.FirstMaterial();
  auto material = ResourcesManager::GetInstance().GetMaterial(materialID);
  if (material == nullptr) {
    return;
  }

  const std::string materialUniform = ShaderUniformsNames::MATERIAL;

  shader.SetUniform(materialUniform + ".ambient", material->ambient);
  shader.SetUniform(materialUniform + ".diffuse", material->diffuse);
  shader.SetUniform(materialUniform + ".specular", material->specular);
  shader.SetUniform(materialUniform + ".shininess", material->shininess);
}

void
ConfigureTextureOnShader(const ResourcesGroup& resources,
                         Rendering::Shader& shader)
{
  if (!resources.HasTextures()) {
    return;
  }

  const TextureID textureID = resources.FirstTexture();
  auto texture = ResourcesManager::GetInstance().GetTexture(textureID);
  if (texture == nullptr) {
    return;
  }

  constexpr auto TEXTURE_UNIT = Chimia::Rendering::TextureUnit::UNIT_1;
  texture->Use(TEXTURE_UNIT);

  shader.SetUniform(ShaderUniformsNames::TEXTURE, TEXTURE_UNIT);
}

void
ConfigureResourceOnShader(const ResourcesGroup& resources,
                          const eVertexLayout& layout,
                          Rendering::Shader& shader)
{
  const bool hasMaterial = resources.HasMaterials();
  const bool hasTexture = resources.HasTextures();

  shader.SetUniform(ShaderUniformsNames::HAS_MATERIAL, hasMaterial);
  shader.SetUniform(ShaderUniformsNames::HAS_TEXTURE, hasTexture);

  if (hasMaterial) {
    ConfigureMaterialOnShader(resources, shader);
  }
  if (hasTexture) {
    ConfigureTextureOnShader(resources, shader);
  }

  ConfigureOpacity(shader, layout, resources);
  ConfigureMixtureColor(shader, resources);
}

void
ConfigureCameraOnShader(Rendering::Shader& shader)
{
  shader.SetUniform(ShaderUniformsNames::CAMERA_TRANSFORM,
                    CameraPrivate::GetCameraTransform());
  shader.SetUniform(ShaderUniformsNames::VIEW_POSITION,
                    CameraPrivate::GetCameraPosition());
}

void
SetDirectionalLightOnShader(const DirectionalLight& light,
                            const int index,
                            Chimia::Rendering::Shader& shader)
{
  const std::string iLight = ShaderUniformsNames::DIRECTIONAL_LIGHTS_ARRAY +
                             "[" + std::to_string(index) + "].";
  const LightColor& col = light.color;

  shader.SetUniform(std::string(iLight + "ambient").c_str(), col.ambient);
  shader.SetUniform(std::string(iLight + "diffuse").c_str(), col.diffuse);
  shader.SetUniform(std::string(iLight + "specular").c_str(), col.specular);
  shader.SetUniform(std::string(iLight + "direction").c_str(), light.direction);
}

void
ConfigureDirectionalLights(Chimia::Rendering::Shader& shader)
{
  int nDirectionalLights = 0;

  const std::vector<DirectionalLight>& directionalLights =
    IlluminationPrivate::GetDirectionalLights();
  for (const DirectionalLight& light : directionalLights) {
    SetDirectionalLightOnShader(light, nDirectionalLights, shader);

    ++nDirectionalLights;
  }

  shader.SetUniform(ShaderUniformsNames::N_DIRECTIONAL_LIGHTS,
                    nDirectionalLights);
}

void
SetPointLightOnShader(const PointLight& light,
                      const int index,
                      Chimia::Rendering::Shader& shader)
{
  const std::string iLight = ShaderUniformsNames::POINT_LIGHTS_ARRAY + "[" +
                             std::to_string(index) + "].";
  const LightColor& col = light.color;
  const PointLightAttenuation& attenuation = light.attenuation;
  shader.SetUniform(std::string(iLight + "ambient").c_str(), col.ambient);
  shader.SetUniform(std::string(iLight + "diffuse").c_str(), col.diffuse);
  shader.SetUniform(std::string(iLight + "specular").c_str(), col.specular);
  shader.SetUniform(std::string(iLight + "position").c_str(), light.position);
  shader.SetUniform(std::string(iLight + "quadratic").c_str(),
                    attenuation.quadratic);
  shader.SetUniform(std::string(iLight + "linear").c_str(), attenuation.linear);
  shader.SetUniform(std::string(iLight + "constant").c_str(),
                    attenuation.constant);
}

void
ConfigurePointLights(Chimia::Rendering::Shader& shader)
{
  int nPointLights = 0;

  const std::vector<PointLight>& pointLights =
    IlluminationPrivate::GetPointLights();
  for (const PointLight& light : pointLights) {
    SetPointLightOnShader(light, nPointLights, shader);

    ++nPointLights;
  }

  shader.SetUniform(ShaderUniformsNames::N_POINT_LIGHTS, nPointLights);
}

void
ConfigureLightsOnShader(Rendering::Shader& shader)
{
  ConfigureDirectionalLights(shader);
  ConfigurePointLights(shader);
}

void
ConfigureShaderForRendering(Rendering::Shader& shader,
                            const eVertexLayout& layout,
                            const bool isInstancedRendering,
                            const ResourcesGroup& resources)
{
  const bool vertexHasNormal = HasNormal(layout);

  shader.SetUniform(ShaderUniformsNames::HAS_VERTEX_COLOR, HasColor(layout));
  shader.SetUniform(ShaderUniformsNames::HAS_NORMAL, vertexHasNormal);
  shader.SetUniform(ShaderUniformsNames::HAS_TEXCOORD, HasTexCoord(layout));
  shader.SetUniform(ShaderUniformsNames::IS_INSTANCED, isInstancedRendering);

  Pipelines::CurrentPipeline().ConfigureShader(shader);

  const int illuminationModel = static_cast<int>(Config::IlluminationModel());
  shader.SetUniform(ShaderUniformsNames::LIGHTNING_MODEL, illuminationModel);

  ConfigureResourceOnShader(resources, layout, shader);
  ConfigureCameraOnShader(shader);
  if (vertexHasNormal) {
    ConfigureLightsOnShader(shader);
  }
}
}

// ----------------------------------------------------------------------------
// RenderersUtils
// ----------------------------------------------------------------------------

VertexLayoutAttributes
RenderersUtils::GetAttributesForLayout(const eVertexLayout& layout)
{
  using namespace RenderersUtilsPrivate;

  switch (layout) {
    case eVertexLayout::POSITION3_COLOR4: {
      return { { PositionAttribute(), ColorAttribute() },
               TransformAttributes() };
    }
    case eVertexLayout::POSITION3_NORMAL3: {
      return { { PositionAttribute(), NormalAttribute() },
               TransformAttributes() };
    }
    case eVertexLayout::POSITION3_TEXCOORD2: {
      return { { PositionAttribute(), TexCoordAttribute() },
               TransformAttributes() };
    }
    case eVertexLayout::POSITION3_COLOR4_NORMAL3: {
      return { { PositionAttribute(), ColorAttribute(), NormalAttribute() },
               TransformAttributes() };
    }
    case eVertexLayout::POSITION3_COLOR4_TEXCOORD2: {
      return { { PositionAttribute(), ColorAttribute(), TexCoordAttribute() },
               TransformAttributes() };
    }
    case eVertexLayout::POSITION3_NORMAL3_TEXCOORD2: {
      return { { PositionAttribute(), NormalAttribute(), TexCoordAttribute() },
               TransformAttributes() };
    }
    case eVertexLayout::POSITION3_COLOR4_NORMAL3_TEXCOORD2: {
      return { { PositionAttribute(),
                 ColorAttribute(),
                 NormalAttribute(),
                 TexCoordAttribute() },
               TransformAttributes() };
    }
    case eVertexLayout::UNDEFINED:
    default: {
      Diagnostics::Error(1, "Unable to fetch attributes for undefined layout");
      return {};
    }
  }
}

// ----------------------------------------------------------------------------

void
RenderersUtils::ConfigureShaderForRendering(Rendering::Shader& shader,
                                            const eVertexLayout& layout,
                                            const ResourcesGroup& resources)
{
  RenderersUtilsPrivate::ConfigureShaderForRendering(
    shader, layout, false /*isInstancedRendering*/, resources);
}

// ----------------------------------------------------------------------------

void
RenderersUtils::ConfigureShaderForInstancedRendering(
  Rendering::Shader& shader,
  const eVertexLayout& layout,
  const ResourcesGroup& resources)
{
  RenderersUtilsPrivate::ConfigureShaderForRendering(
    shader, layout, true /*isInstancedRendering*/, resources);
}

// ----------------------------------------------------------------------------
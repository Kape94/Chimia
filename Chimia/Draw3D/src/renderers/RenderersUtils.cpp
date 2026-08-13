#include "RenderersUtils.h"

#include "CameraPrivate.h"
#include "Config.h"
#include "DataNames.h"
#include "IlluminationPrivate.h"
#include "Pipelines.h"
#include "Rendering/Target.h"
#include "ResourceGroup.h"
#include "ResourcesManager.h"
#include "Shaders.h"
#include "Types.h"

#include "Rendering/Shader.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
// RenderersUtilsPrivate
// ----------------------------------------------------------------------------

namespace RenderersUtilsPrivate {
using namespace Chimia;

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
ConfigureOpacity(Rendering::ShaderInstance& shader,
                 const eVertexLayout& layout,
                 const ResourcesGroup& resources)
{
  const bool allowsOpacity = !HasNormal(layout) && resources.HasOpacityFactor();

  if (allowsOpacity) {
    const OpacityFactorID opacityID = resources.FirstOpacityFactor();
    const float* opacity =
      ResourcesManager::GetInstance().GetOpacityFactor(opacityID);

    shader->SetUniform(DataNames::ShaderUniforms::OPACITY, *opacity);
  } else {
    shader->SetUniform(DataNames::ShaderUniforms::OPACITY, 1.0f);
  }
}

void
ConfigureMixtureColor(Rendering::ShaderInstance& shader,
                      const ResourcesGroup& resources)
{
  if (resources.HasMixtureColor()) {
    const MixtureColorID colorID = resources.FirstMixtureColor();
    const glm::vec3* color =
      ResourcesManager::GetInstance().GetMixtureColor(colorID);

    shader->SetUniform(DataNames::ShaderUniforms::MIXTURE_COLOR, *color);

  } else {
    shader->SetUniform(DataNames::ShaderUniforms::MIXTURE_COLOR,
                       glm::vec3(1.0f, 1.0f, 1.0f));
  }
}

void
ConfigureMaterialOnShader(const ResourcesGroup& resources,
                          Rendering::ShaderInstance& shader)
{
  if (!resources.HasMaterials()) {
    return;
  }

  const MaterialID materialID = resources.FirstMaterial();
  auto material = ResourcesManager::GetInstance().GetMaterial(materialID);
  if (material == nullptr) {
    return;
  }

  const std::string materialUniform = DataNames::ShaderUniforms::MATERIAL;

  shader->SetUniform(materialUniform + ".ambient", material->ambient);
  shader->SetUniform(materialUniform + ".diffuse", material->diffuse);
  shader->SetUniform(materialUniform + ".specular", material->specular);
  shader->SetUniform(materialUniform + ".shininess", material->shininess);
}

void
ConfigureTextureOnShader(const ResourcesGroup& resources,
                         Rendering::ShaderInstance& shader)
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
  shader->SetTexture(
    DataNames::ShaderUniforms::TEXTURE, *texture, TEXTURE_UNIT);
}

void
ConfigureResourceOnShader(const ResourcesGroup& resources,
                          const eVertexLayout& layout,
                          Rendering::ShaderInstance& shader)
{
  const bool hasMaterial = resources.HasMaterials();
  const bool hasTexture = resources.HasTextures();

  shader->SetUniform(DataNames::ShaderUniforms::HAS_MATERIAL, hasMaterial);
  shader->SetUniform(DataNames::ShaderUniforms::HAS_TEXTURE, hasTexture);

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
ConfigureCameraOnShader(Rendering::ShaderInstance& shader)
{
  shader->SetUniform(DataNames::ShaderUniforms::CAMERA_TRANSFORM,
                     CameraPrivate::GetCameraTransform());
  shader->SetUniform(DataNames::ShaderUniforms::VIEW_POSITION,
                     CameraPrivate::GetCameraPosition());
}

void
SetDirectionalLightOnShader(const DirectionalLight& light,
                            const int index,
                            Chimia::Rendering::ShaderInstance& shader)
{
  const std::string iLight =
    DataNames::ShaderUniforms::DIRECTIONAL_LIGHTS_ARRAY + "[" +
    std::to_string(index) + "].";
  const LightColor& col = light.color;

  shader->SetUniform(std::string(iLight + "ambient").c_str(), col.ambient);
  shader->SetUniform(std::string(iLight + "diffuse").c_str(), col.diffuse);
  shader->SetUniform(std::string(iLight + "specular").c_str(), col.specular);
  shader->SetUniform(std::string(iLight + "direction").c_str(),
                     light.direction);
}

void
ConfigureDirectionalLights(Chimia::Rendering::ShaderInstance& shader)
{
  int nDirectionalLights = 0;

  const std::vector<DirectionalLight>& directionalLights =
    IlluminationPrivate::GetDirectionalLights();
  for (const DirectionalLight& light : directionalLights) {
    SetDirectionalLightOnShader(light, nDirectionalLights, shader);

    ++nDirectionalLights;
  }

  shader->SetUniform(DataNames::ShaderUniforms::N_DIRECTIONAL_LIGHTS,
                     nDirectionalLights);
}

void
SetPointLightOnShader(const PointLight& light,
                      const int index,
                      Chimia::Rendering::ShaderInstance& shader)
{
  const std::string iLight = DataNames::ShaderUniforms::POINT_LIGHTS_ARRAY +
                             "[" + std::to_string(index) + "].";
  const LightColor& col = light.color;
  const PointLightAttenuation& attenuation = light.attenuation;
  shader->SetUniform(std::string(iLight + "ambient").c_str(), col.ambient);
  shader->SetUniform(std::string(iLight + "diffuse").c_str(), col.diffuse);
  shader->SetUniform(std::string(iLight + "specular").c_str(), col.specular);
  shader->SetUniform(std::string(iLight + "position").c_str(), light.position);
  shader->SetUniform(std::string(iLight + "quadratic").c_str(),
                     attenuation.quadratic);
  shader->SetUniform(std::string(iLight + "linear").c_str(),
                     attenuation.linear);
  shader->SetUniform(std::string(iLight + "constant").c_str(),
                     attenuation.constant);
}

void
ConfigurePointLights(Chimia::Rendering::ShaderInstance& shader)
{
  int nPointLights = 0;

  const std::vector<PointLight>& pointLights =
    IlluminationPrivate::GetPointLights();
  for (const PointLight& light : pointLights) {
    SetPointLightOnShader(light, nPointLights, shader);

    ++nPointLights;
  }

  shader->SetUniform(DataNames::ShaderUniforms::N_POINT_LIGHTS, nPointLights);
}

void
ConfigureLightsOnShader(Rendering::ShaderInstance& shader)
{
  ConfigureDirectionalLights(shader);
  ConfigurePointLights(shader);
}

void
ConfigureShaderForRendering(Rendering::ShaderInstance& shader,
                            const eVertexLayout& layout,
                            const bool isInstancedRendering,
                            const ResourcesGroup& resources)
{
  const bool vertexHasNormal = HasNormal(layout);

  shader->SetUniform(DataNames::ShaderUniforms::HAS_VERTEX_COLOR,
                     HasColor(layout));
  shader->SetUniform(DataNames::ShaderUniforms::HAS_NORMAL, vertexHasNormal);
  shader->SetUniform(DataNames::ShaderUniforms::HAS_TEXCOORD,
                     HasTexCoord(layout));
  shader->SetUniform(DataNames::ShaderUniforms::IS_INSTANCED,
                     isInstancedRendering);

  Pipelines::CurrentPipeline().ConfigureShader(shader);

  const int illuminationModel = static_cast<int>(Config::IlluminationModel());
  shader->SetUniform(DataNames::ShaderUniforms::LIGHTNING_MODEL,
                     illuminationModel);

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

bool
RenderersUtils::HasColor(const eVertexLayout& layout)
{
  return RenderersUtilsPrivate::HasColor(layout);
}

// ----------------------------------------------------------------------------

bool
RenderersUtils::HasNormal(const eVertexLayout& layout)
{
  return RenderersUtilsPrivate::HasNormal(layout);
}

// ----------------------------------------------------------------------------

bool
RenderersUtils::HasTexCoord(const eVertexLayout& layout)
{
  return RenderersUtilsPrivate::HasTexCoord(layout);
}

// ----------------------------------------------------------------------------

const Chimia::Rendering::TargetInstance&
RenderersUtils::GetDefaultRenderingTarget()
{
  static Rendering::TargetInstance defaultTarget =
    Rendering::Target::Create(Shaders::Generic());

  return defaultTarget;
}

// ----------------------------------------------------------------------------

void
RenderersUtils::ConfigureShaderForRendering(Rendering::ShaderInstance& shader,
                                            const eVertexLayout& layout,
                                            const ResourcesGroup& resources)
{
  shader->SetUniform(DataNames::ShaderUniforms::IS_TRANSITION_RENDERING, false);
  RenderersUtilsPrivate::ConfigureShaderForRendering(
    shader, layout, false /*isInstancedRendering*/, resources);
}

// ----------------------------------------------------------------------------

void
RenderersUtils::ConfigureShaderForInstancedRendering(
  Rendering::ShaderInstance& shader,
  const eVertexLayout& layout,
  const ResourcesGroup& resources)
{
  shader->SetUniform(DataNames::ShaderUniforms::IS_TRANSITION_RENDERING, false);
  RenderersUtilsPrivate::ConfigureShaderForRendering(
    shader, layout, true /*isInstancedRendering*/, resources);
}

// ----------------------------------------------------------------------------

void
RenderersUtils::ConfigureForTransitionRendering(
  Rendering::ShaderInstance& shader)
{
  shader->SetUniform(DataNames::ShaderUniforms::IS_TRANSITION_RENDERING, true);
}

// ----------------------------------------------------------------------------
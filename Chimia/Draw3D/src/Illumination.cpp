#include "Illumination.h"
#include "IlluminationPrivate.h"

#include "ShaderUniformsNames.h"

#include "CameraPrivate.h"
#include "Rendering/Shader.h"

#include <vector>

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace IlluminationInternal {

// ----------------------------------------------------------------------------

namespace State {
// TODO: is worth enabling disabling light?
bool enabled = false;

// TODO: should lights be persistent? Should we have both?
std::vector<DirectionalLight> directionalLights;
std::vector<PointLight> pointLights;
}

// ----------------------------------------------------------------------------

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

// ----------------------------------------------------------------------------

void
ConfigureDirectionalLights(Chimia::Rendering::Shader& shader)
{
  int nDirectionalLights = 0;
  for (const DirectionalLight& light : State::directionalLights) {
    SetDirectionalLightOnShader(light, nDirectionalLights, shader);

    ++nDirectionalLights;
  }

  shader.SetUniform(ShaderUniformsNames::N_DIRECTIONAL_LIGHTS,
                    nDirectionalLights);
}

// ----------------------------------------------------------------------------

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

// ----------------------------------------------------------------------------

void
ConfigurePointLights(Chimia::Rendering::Shader& shader)
{
  int nPointLights = 0;
  for (const PointLight& light : State::pointLights) {
    SetPointLightOnShader(light, nPointLights, shader);

    ++nPointLights;
  }

  shader.SetUniform(ShaderUniformsNames::N_POINT_LIGHTS, nPointLights);
}

}

// ----------------------------------------------------------------------------
// Illumination public
// ----------------------------------------------------------------------------

void
Chimia::Draw3D::EnableLights(const bool enable)
{
  IlluminationInternal::State::enabled = enable;
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::SetLight(const DirectionalLight& light)
{
  IlluminationInternal::State::directionalLights.push_back(light);
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::SetLight(const PointLight& light)
{
  IlluminationInternal::State::pointLights.push_back(light);
}

// ----------------------------------------------------------------------------
// Illumination private
// ----------------------------------------------------------------------------

void
IlluminationPrivate::ConfigureLightsOnShader(Rendering::Shader& shader)
{

  IlluminationInternal::ConfigureDirectionalLights(shader);
  IlluminationInternal::ConfigurePointLights(shader);

  CameraPrivate::SetPointOfViewOnShader(shader);
  CameraPrivate::SetCameraOnShader(shader);
}

// ----------------------------------------------------------------------------

void
IlluminationPrivate::ConfigureMaterialOnShader(const Material& material,
                                               Rendering::Shader& shader)
{
  const std::string materialUniform = ShaderUniformsNames::MATERIAL;

  shader.SetUniform(materialUniform + ".ambient", material.ambient);
  shader.SetUniform(materialUniform + ".diffuse", material.diffuse);
  shader.SetUniform(materialUniform + ".specular", material.specular);
  shader.SetUniform(materialUniform + ".shininess", material.shininess);
}

// ----------------------------------------------------------------------------
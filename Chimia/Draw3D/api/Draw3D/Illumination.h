#pragma once

#include "Draw3DNamespaceDefs.h"

#include <glm/vec3.hpp>

BEGIN_CHIMIA_DRAW3D_NAMESPACE

struct LightColor
{
  glm::vec3 ambient{ 0.0f, 0.0f, 0.0f };
  glm::vec3 diffuse{ 0.0f, 0.0f, 0.0f };
  glm::vec3 specular{ 0.0f, 0.0f, 0.0f };
};

struct PointLightAttenuation
{
  float quadratic = 0.0f;
  float linear = 0.0f;
  float constant = 0.0f;
};

struct DirectionalLight
{
  glm::vec3 direction{ 0.0f, 0.0f, 0.0f };
  LightColor color;
};

struct PointLight
{
  glm::vec3 position{ 0.0f, 0.0f, 0.0f };
  LightColor color;
  PointLightAttenuation attenuation;
};

struct Material
{
  glm::vec3 ambient{ 0.0f, 0.0f, 0.0f };
  glm::vec3 diffuse{ 0.0f, 0.0f, 0.0f };
  glm::vec3 specular{ 0.0f, 0.0f, 0.0f };
  float shininess;
};

void
EnableLights(const bool enable);

void
SetLight(const DirectionalLight& light);

void
SetLight(const PointLight& light);

END_CHIMIA_DRAW3D_NAMESPACE
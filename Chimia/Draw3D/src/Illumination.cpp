#include "Illumination.h"
#include "IlluminationPrivate.h"

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

const std::vector<DirectionalLight>&
IlluminationPrivate::GetDirectionalLights()
{
  return IlluminationInternal::State::directionalLights;
}

// ----------------------------------------------------------------------------

const std::vector<PointLight>&
IlluminationPrivate::GetPointLights()
{
  return IlluminationInternal::State::pointLights;
}

// ----------------------------------------------------------------------------
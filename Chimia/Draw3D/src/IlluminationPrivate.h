#pragma once

#include "Draw3DNamespaceDefs.h"

#include "Types.h"

BEGIN_CHIMIA_DRAW3D_NAMESPACE

namespace IlluminationPrivate {

const std::vector<DirectionalLight>&
GetDirectionalLights();

const std::vector<PointLight>&
GetPointLights();

}

END_CHIMIA_DRAW3D_NAMESPACE
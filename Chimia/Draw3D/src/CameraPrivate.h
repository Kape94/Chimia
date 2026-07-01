#pragma once

#include "Draw3DNamespaceDefs.h"

#include <glm/ext/matrix_float4x4.hpp>

BEGIN_CHIMIA_DRAW3D_NAMESPACE

namespace CameraPrivate {

glm::mat4x4
GetCameraTransform();

glm::vec3
GetCameraPosition();

}

END_CHIMIA_DRAW3D_NAMESPACE
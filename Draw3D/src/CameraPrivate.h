#pragma once

#include "Draw3DNamespaceDefs.h"

#include <glm/ext/matrix_float4x4.hpp>

BEGIN_CHIMIA_DRAW3D_NAMESPACE

namespace CameraPrivate {
glm::mat4x4
GetCurrentTransform();
}

END_CHIMIA_DRAW3D_NAMESPACE
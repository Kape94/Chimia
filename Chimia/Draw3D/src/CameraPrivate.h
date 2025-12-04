#pragma once

#include "Draw3DNamespaceDefs.h"

#include "Rendering/Shader.h"

#include <glm/ext/matrix_float4x4.hpp>

BEGIN_CHIMIA_DRAW3D_NAMESPACE

namespace CameraPrivate {
void
SetCameraOnShader(Rendering::Shader& shader);

void
SetPointOfViewOnShader(Rendering::Shader& shader);
}

END_CHIMIA_DRAW3D_NAMESPACE
#pragma once

#include "Draw3DNamespaceDefs.h"
#include "InternalTypes.h"

#include "Rendering/Shader.h"

BEGIN_CHIMIA_DRAW3D_NAMESPACE

namespace IlluminationPrivate {
void
ConfigureLightsOnShader(Rendering::Shader& shader);

void
ConfigureMaterialOnShader(const Material& material, Rendering::Shader& shader);
}

END_CHIMIA_DRAW3D_NAMESPACE
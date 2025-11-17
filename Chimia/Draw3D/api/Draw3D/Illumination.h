#pragma once

#include "Draw3DNamespaceDefs.h"

#include "Types.h"

BEGIN_CHIMIA_DRAW3D_NAMESPACE

void
EnableLights(const bool enable);

void
SetLight(const DirectionalLight& light);

void
SetLight(const PointLight& light);

END_CHIMIA_DRAW3D_NAMESPACE
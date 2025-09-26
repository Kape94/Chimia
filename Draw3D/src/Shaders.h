#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include "Rendering/Shader.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

namespace Shaders {
void
Initialize();

Rendering::Shader&
VertexColored();
}

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

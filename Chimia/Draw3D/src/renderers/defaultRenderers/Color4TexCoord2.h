#pragma once

// ----------------------------------------------------------------------------

#include "DefaultRenderersNamespaceDefs.h"
#include "Draw3DNamespaceDefs.h"
#include "GenericRenderer.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

BEGIN_DEFAULT_RENDERERS_NAMESPACE

namespace Color4TexCoord2 {
void
Init();

void
Shutdown();

GenericRenderer&
GetRenderer();

GenericRenderer&
GetTransparentRenderer();

};

END_DEFAULT_RENDERERS_NAMESPACE

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
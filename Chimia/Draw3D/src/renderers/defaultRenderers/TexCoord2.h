#pragma once

// ----------------------------------------------------------------------------

#include "DefaultRenderersNamespaceDefs.h"
#include "Draw3DNamespaceDefs.h"
#include "GenericRenderer.h"
#include "Types.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

BEGIN_DEFAULT_RENDERERS_NAMESPACE

namespace TexCoord2 {
void
Init();

GenericRenderer&
GetRenderer();

void
DrawTriangle(const glm::vec3& p1,
             const glm::vec2& p1TexCoord,
             const glm::vec3& p2,
             const glm::vec2& p2TexCoord,
             const glm::vec3& p3,
             const glm::vec2& p3TexCoord,
             const ResourceGroupID& resources);
}

END_DEFAULT_RENDERERS_NAMESPACE

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
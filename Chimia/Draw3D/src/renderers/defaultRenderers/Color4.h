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

namespace Color4 {
void
Init();

GenericRenderer&
GetRenderer();

void
DrawTriangle(const glm::vec3& p1,
             const glm::vec4& color1,
             const glm::vec3& p2,
             const glm::vec4& color2,
             const glm::vec3& p3,
             const glm::vec4& color3,
             const ResourceGroupID& resource);
};

END_DEFAULT_RENDERERS_NAMESPACE

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
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

namespace Color3Normal3 {
void
Init();

GenericRenderer&
GetRenderer();

void
DrawTriangle(const glm::vec3& p1,
             const glm::vec4& p1Color,
             const glm::vec3& p1Normal,
             const glm::vec3& p2,
             const glm::vec4& p2Color,
             const glm::vec3& p2Normal,
             const glm::vec3& p3,
             const glm::vec4& p3Color,
             const glm::vec3& p3Normal,
             const ResourceGroupID& resource);
};

END_DEFAULT_RENDERERS_NAMESPACE

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
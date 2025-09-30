#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include <glm/vec3.hpp>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

namespace TrianglePrivate {
void
Init();

void
Flush();

void
Draw(const glm::vec3& p1,
     const glm::vec3& p1Color,
     const glm::vec3& p2,
     const glm::vec3& p2Color,
     const glm::vec3& p3,
     const glm::vec3& p3Color);
}

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

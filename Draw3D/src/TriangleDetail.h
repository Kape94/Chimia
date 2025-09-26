#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include <glm/vec3.hpp>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

namespace TriangleDetail {
void
Init();

void
Flush();

void
Draw(const glm::vec3& p1,
     const glm::vec3& p2,
     const glm::vec3& p3,
     const glm::vec3& color);
}

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

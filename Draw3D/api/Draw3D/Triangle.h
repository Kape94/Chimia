#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include <glm/vec3.hpp>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

void
Init();

void
Triangle(const glm::vec3& p1,
         const glm::vec3& p2,
         const glm::vec3& p3,
         const glm::vec3& color);

void
Flush();

void
ClearScreen();

END_CHIMIA_DRAW3D_NAMESPACE

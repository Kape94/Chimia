#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "Types.h"

#include "Core/Types.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

void
Triangle(const glm::vec3& p1,
         const glm::vec3& p2,
         const glm::vec3& p3,
         const glm::vec3& color);

void
Triangle(const glm::vec3& p1,
         const glm::vec3& p1Color,
         const glm::vec3& p2,
         const glm::vec3& p2Color,
         const glm::vec3& p3,
         const glm::vec3& p3Color);

void
LitTriangle(const glm::vec3& p1,
            const glm::vec3& p1Normal,
            const glm::vec3& p2,
            const glm::vec3& p2Normal,
            const glm::vec3& p3,
            const glm::vec3& p3Normal,
            const MaterialID& material);

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
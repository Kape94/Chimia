#pragma once

#include "Draw3DNamespaceDefs.h"
#include "Illumination.h"

#include <glm/vec3.hpp>

BEGIN_CHIMIA_DRAW3D_NAMESPACE

void
LitTriangle(const glm::vec3& p1,
            const glm::vec3& p1Normal,
            const glm::vec3 p2,
            const glm::vec3& p2Normal,
            const glm::vec3 p3,
            const glm::vec3& p3Normal,
            const Material& material);

END_CHIMIA_DRAW3D_NAMESPACE
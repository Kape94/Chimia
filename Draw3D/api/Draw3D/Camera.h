#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include <glm/vec3.hpp>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

namespace Camera::Projection {
void
SetIdentity();

void
SetPerspective(const float fieldOfView,
               const float aspect,
               const float nearClippingPlane,
               const float farClippingPlane);

void
SetOthographic(const float left,
               const float right,
               const float bottom,
               const float top,
               const float near,
               const float far);
}

namespace Camera::View {
void
LookAt(const glm::vec3& eye, const glm::vec3& target);
}

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
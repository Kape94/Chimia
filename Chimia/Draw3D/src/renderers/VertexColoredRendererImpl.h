#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "GenericRenderer.h"
#include "Types.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

namespace VertexColoredRendererImpl {
GenericRenderer&
GetRenderer();

void
DrawTriangle(const glm::vec3& p1,
             const glm::vec3& color1,
             const glm::vec3& p2,
             const glm::vec3& color2,
             const glm::vec3& p3,
             const glm::vec3& color3,
             const ResourceGroupID& resource);
};

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
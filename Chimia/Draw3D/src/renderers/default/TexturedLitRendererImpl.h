#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "GenericRenderer.h"
#include "Types.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

namespace TexturedLitRendererImpl {
void
Init();

GenericRenderer&
GetRenderer();

void
DrawTriangle(const glm::vec3& p1,
             const glm::vec3& p1Normal,
             const glm::vec2& p1TexCoord,
             const glm::vec3& p2,
             const glm::vec3& p2Normal,
             const glm::vec2& p2TexCoord,
             const glm::vec3& p3,
             const glm::vec3& p3Normal,
             const glm::vec2& p3TexCoord,
             const ResourceGroupID& resource);
};

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
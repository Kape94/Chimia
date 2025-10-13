#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include <glm/vec3.hpp>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
// Renderers interfaces
// ----------------------------------------------------------------------------

class VertexColoredRenderer
{
public:
  virtual void DrawTriangle(const glm::vec3& p1,
                            const glm::vec3& color1,
                            const glm::vec3& p2,
                            const glm::vec3& color2,
                            const glm::vec3& p3,
                            const glm::vec3& color3) = 0;
};

// ----------------------------------------------------------------------------
// Renderer retrievers
// ----------------------------------------------------------------------------

VertexColoredRenderer&
GetVertexColoredRenderer();

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
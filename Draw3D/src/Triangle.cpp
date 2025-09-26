#include "Triangle.h"

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "TriangleDetail.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangle(const glm::vec3& p1,
                         const glm::vec3& p2,
                         const glm::vec3& p3,
                         const glm::vec3& color)
{
  Triangle(p1, color, p2, color, p3, color);
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangle(const glm::vec3& p1,
                         const glm::vec3& p1Color,
                         const glm::vec3& p2,
                         const glm::vec3& p2Color,
                         const glm::vec3& p3,
                         const glm::vec3& p3Color)
{
  TriangleDetail::Draw(p1, p1Color, p2, p2Color, p3, p3Color);
}

// ----------------------------------------------------------------------------

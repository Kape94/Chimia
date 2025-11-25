#include "Triangle.h"

// ----------------------------------------------------------------------------

#include "Core/Types.h"
#include "Draw3DNamespaceDefs.h"
#include "LitRendererImpl.h"
#include "VertexColoredRendererImpl.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace {
auto& renderer = VertexColoredRendererImpl::getInstance();
auto& litRenderer = LitRendererImpl::getInstance();
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangle(const Position3& p1,
                         const Position3& p2,
                         const Position3& p3,
                         const Color3& color)
{
  Triangle(p1, color, p2, color, p3, color);
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangle(const Position3& p1,
                         const Color3& p1Color,
                         const Position3& p2,
                         const Color3& p2Color,
                         const Position3& p3,
                         const Color3& p3Color)
{
  renderer.DrawTriangle(p1, p1Color, p2, p2Color, p3, p3Color);
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::LitTriangle(const Position3& p1,
                            const Normal3& p1Normal,
                            const Position3& p2,
                            const Normal3& p2Normal,
                            const Position3& p3,
                            const Normal3& p3Normal,
                            const MaterialID& material)
{
  litRenderer.DrawTriangle(p1, p1Normal, p2, p2Normal, p3, p3Normal, material);
}

// ----------------------------------------------------------------------------

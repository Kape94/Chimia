#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "Types.h"

#include "Core/Types.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

void
Triangle(const Position3& p1,
         const Position3& p2,
         const Position3& p3,
         const Color3& color);

void
Triangle(const Position3& p1,
         const Color3& p1Color,
         const Position3& p2,
         const Color3& p2Color,
         const Position3& p3,
         const Color3& p3Color);

void
LitTriangle(const Position3& p1,
            const Normal3& p1Normal,
            const Position3& p2,
            const Normal3& p2Normal,
            const Position3& p3,
            const Normal3& p3Normal,
            const MaterialID& material);

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
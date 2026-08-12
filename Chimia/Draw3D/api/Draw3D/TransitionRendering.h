#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include "Types.h"

#include <glm/mat4x4.hpp>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

// Model Position3 + Color4
// Model Position3 + Color4 + Normal3

void
DrawTransition(const TransitionID& transition,
               const float interpolation,
               const glm::mat4x4& transform);

// Model Position3 + Normal3

void
DrawTransition(const TransitionID& transition,
               const float interpolation,
               const glm::mat4x4& transform,
               const MaterialID& materialID);

// Model Position3 + TexCoord2
// Model Position3 + Color4 + TexCoord2
// Model Position3 + Normal3 + TexCoord2
// Model Position3 + Color4 + Normal3 + TexCoord2

void
DrawTransition(const TransitionID& transition,
               const float interpolation,
               const glm::mat4x4& transform,
               const TextureID& textureID);

// General

void
DrawTransition(const TransitionID& transition,
               const float interpolation,
               const glm::mat4x4& transform,
               const ResourceGroupID& resource);

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
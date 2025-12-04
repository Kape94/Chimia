#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include "Types.h"

#include <glm/mat4x4.hpp>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

// Model Position3 + Color3
// Model Position3 + COlor3 + Normal3

void
DrawModel(const ModelID& modelID, const glm::mat4x4& transform);

ModelInstanceID
AddStaticModel(const ModelID& modelID, const glm::mat4x4& transform);

// Model Position3 + Normal3

void
DrawModel(const ModelID& modelID,
          const glm::mat4x4& transform,
          const MaterialID& materialID);

ModelInstanceID
AddStaticModel(const ModelID& modelID,
               const glm::mat4x4& transform,
               const MaterialID& materialID);

// Model Position3 + TexCoord2

void
DrawModel(const ModelID& modelID,
          const glm::mat4x4& transform,
          const TextureID& textureID);

ModelInstanceID
AddStaticModel(const ModelID& modelID,
               const glm::mat4x4& transform,
               const TextureID& textureID);

// General

void
DeleteStaticModel(const ModelInstanceID& instanceID);

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
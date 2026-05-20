#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include "Types.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace ResourceGroupHelper {

ResourceGroupID
GetEmptyResource();

ResourceGroupID
WrapTexture(const TextureID& texture);
ResourceGroupID
GetResourceGroup(const TextureID& texture);

ResourceGroupID
WrapMaterial(const MaterialID& material);
ResourceGroupID
GetResourceGroup(const MaterialID& material);

bool
HasMaterial(const ResourceGroupID& resource);
bool
HasTexture(const ResourceGroupID& resource);
bool
HasOpacityFactor(const ResourceGroupID& resource);

}

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
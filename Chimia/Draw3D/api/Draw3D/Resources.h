#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include "Types.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

ModelID
CreateModel(const MeshDataView& meshData, const eVertexLayout vertexLayout);

MaterialID
CreateMaterial(const glm::vec3& ambient,
               const glm::vec3& diffuse,
               const glm::vec3& specular,
               const float shininess);

TextureID
CreateTexture(const unsigned char* textureData,
              const unsigned width,
              const unsigned height);

OpacityFactorID
CreateOpacityFactor(const float value);

ResourceGroupID
CreateResourceGroup();

void
AddResourceToGroup(const MaterialID& material, const ResourceGroupID& group);

void
AddResourceToGroup(const TextureID& texture, const ResourceGroupID& group);

void
AddResourceToGroup(const OpacityFactorID& opacityFactor,
                   const ResourceGroupID& group);

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
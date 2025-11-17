#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include "Types.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

ModelID
CreateModel(const MeshDataView& meshData);

MaterialID
CreateMaterial(const glm::vec3& ambient,
               const glm::vec3& diffuse,
               const glm::vec3& specular,
               const float shininess);

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
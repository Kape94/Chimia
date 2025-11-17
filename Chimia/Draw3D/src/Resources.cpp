#include "Resources.h"

#include "ResourcesManager.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

ModelID
CHIMIA_DRAW3D_NAMESPACE_NAME::CreateModel(const MeshDataView& meshData)
{
  auto& manager = ResourcesManager::GetInstance();
  return manager.CreateModel(meshData);
}

// ----------------------------------------------------------------------------

MaterialID
CHIMIA_DRAW3D_NAMESPACE_NAME::CreateMaterial(const glm::vec3& ambient,
                                             const glm::vec3& diffuse,
                                             const glm::vec3& specular,
                                             const float shininess)
{
  auto& manager = ResourcesManager::GetInstance();
  return manager.CreateMaterial(ambient, diffuse, specular, shininess);
}

// ----------------------------------------------------------------------------

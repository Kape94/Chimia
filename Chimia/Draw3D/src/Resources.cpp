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

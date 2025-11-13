#include "ResourcesManager.h"

#include "Draw3DPrivate.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

ResourcesManager&
ResourcesManager::GetInstance()
{
  static ResourcesManager instance;
  return instance;
}

// ----------------------------------------------------------------------------

ModelID
ResourcesManager::CreateModel(const MeshDataView& meshData)
{
  auto [modelIDValue, model] = m_modelsTable.Insert();

  model->Create(meshData);

  return Draw3DPrivate::CreateModelID(modelIDValue);
}

// ----------------------------------------------------------------------------

const Model*
ResourcesManager::GetModel(const ModelID& modelID) const
{
  return m_modelsTable.Find(Draw3DPrivate::GetModelID(modelID));
}

// ----------------------------------------------------------------------------
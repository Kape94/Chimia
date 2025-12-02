#include "ResourcesManager.h"

#include "Draw3DPrivate.h"
#include "Types.h"

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
ResourcesManager::CreateModel(const MeshDataView& meshData,
                              const eVertexLayout vertexLayout)
{
  auto [modelIDValue, model] = m_modelsTable.Insert();

  model->Create(meshData, vertexLayout);

  return Draw3DPrivate::CreateModelID(modelIDValue);
}

// ----------------------------------------------------------------------------

const Model*
ResourcesManager::GetModel(const ModelID& modelID) const
{
  return m_modelsTable.Find(Draw3DPrivate::GetModelID(modelID));
}

// ----------------------------------------------------------------------------

MaterialID
ResourcesManager::CreateMaterial(const glm::vec3& ambient,
                                 const glm::vec3& diffuse,
                                 const glm::vec3& specular,
                                 const float shininess)
{
  auto [materialID, material] = m_materialsTable.Insert();

  material->ambient = ambient;
  material->diffuse = diffuse;
  material->specular = specular;
  material->shininess = shininess;

  return Draw3DPrivate::CreateMaterialID(materialID);
}

// ----------------------------------------------------------------------------

const Material*
ResourcesManager::GetMaterial(const MaterialID& materialID)
{
  return m_materialsTable.Find(Draw3DPrivate::GetMaterialIDValue(materialID));
}

// ----------------------------------------------------------------------------
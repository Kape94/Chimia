#include "ResourcesManager.h"

#include "Core/Diagnostics.h"

#include "Draw3DPrivate.h"
#include "Rendering/Texture2D.h"
#include "Types.h"
#include "resources/ResourceGroup.h"

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

TextureID
ResourcesManager::CreateTexture(const unsigned char* textureData,
                                const unsigned width,
                                const unsigned height)
{
  auto [id, texture] = m_texturesTable.Insert();

  texture->Create(textureData, width, height);

  return Draw3DPrivate::CreateTextureID(id);
}

// ----------------------------------------------------------------------------

Chimia::Rendering::Texture2D*
ResourcesManager::GetTexture(const TextureID& textureID)
{
  const unsigned idValue = Draw3DPrivate::GetTextureIDValue(textureID);
  return m_texturesTable.Find(idValue);
}

// ----------------------------------------------------------------------------

OpacityFactorID
ResourcesManager::CreateOpacityFactor(const float value)
{
  auto [id, newFactor] = m_opacityFactors.Insert();
  *newFactor = value;

  return Draw3DPrivate::CreateOpacityFactorID(id);
}

// ----------------------------------------------------------------------------

const float*
ResourcesManager::GetOpacityFactor(const OpacityFactorID& opacityFactorID)
{
  const unsigned idValue =
    Draw3DPrivate::GetOpacityFactorIDValue(opacityFactorID);
  return m_opacityFactors.Find(idValue);
}

// ----------------------------------------------------------------------------

ResourceGroupID
ResourcesManager::CreateResourceGroup()
{
  auto [id, _] = m_resourceGroups.Insert();
  return Draw3DPrivate::CreateResourceGroupID(id);
}

// ----------------------------------------------------------------------------

void
ResourcesManager::AddResourceToGroup(const std::string& tag,
                                     const MaterialID& material,
                                     const ResourceGroupID& group)
{
  const unsigned idValue = Draw3DPrivate::GetResourceGroupIDValue(group);
  ResourcesGroup* groupInstance = m_resourceGroups.Find(idValue);
  if (groupInstance == nullptr) {
    Chimia::Diagnostics::Error(1, "Didn't find group instance");
  }

  groupInstance->AddResource(tag, material);
}

// ----------------------------------------------------------------------------

void
ResourcesManager::AddResourceToGroup(const std::string& tag,
                                     const TextureID& texture,
                                     const ResourceGroupID& group)
{
  const unsigned idValue = Draw3DPrivate::GetResourceGroupIDValue(group);
  ResourcesGroup* groupInstance = m_resourceGroups.Find(idValue);
  if (groupInstance == nullptr) {
    Chimia::Diagnostics::Error(1, "Didn't find group instance");
  }

  groupInstance->AddResource(tag, texture);
}

// ----------------------------------------------------------------------------

void
ResourcesManager::AddResourceToGroup(const std::string& tag,
                                     const OpacityFactorID& opacityFactor,
                                     const ResourceGroupID& group)
{
  const unsigned idValue = Draw3DPrivate::GetResourceGroupIDValue(group);
  ResourcesGroup* groupInstance = m_resourceGroups.Find(idValue);
  if (groupInstance == nullptr) {
    Chimia::Diagnostics::Error(1, "Didn't find group instance");
  }

  groupInstance->AddResource(tag, opacityFactor);
}

// ----------------------------------------------------------------------------

const ResourcesGroup*
ResourcesManager::GetResourcesGroup(const ResourceGroupID& groupID)
{
  const unsigned idValue = Draw3DPrivate::GetResourceGroupIDValue(groupID);
  return m_resourceGroups.Find(idValue);
}

// ----------------------------------------------------------------------------
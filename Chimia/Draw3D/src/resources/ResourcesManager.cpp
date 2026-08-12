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

TransitionID
ResourcesManager::CreateTransition(const ModelID& start, const ModelID& target)
{
  auto [transitionIDValue, transition] =
    m_transitionsTable.Insert(start, target);

  return Draw3DPrivate::CreateTransitionID(transitionIDValue);
}

// ----------------------------------------------------------------------------

const Transition*
ResourcesManager::GetTransition(const TransitionID& transitionID) const
{
  return m_transitionsTable.Find(
    Draw3DPrivate::GetTransitionIDValue(transitionID));
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

  (*texture) = Rendering::Texture2D::Create(textureData, width, height);

  return Draw3DPrivate::CreateTextureID(id);
}

// ----------------------------------------------------------------------------

const Chimia::Rendering::Texture2DInstance*
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

void
ResourcesManager::UpdateOpacityFactor(const OpacityFactorID& opacityID,
                                      const float newValue)
{
  if (float* opacityValue = const_cast<float*>(GetOpacityFactor(opacityID))) {
    *opacityValue = newValue;
  }
}

// ----------------------------------------------------------------------------

MixtureColorID
ResourcesManager::CreateMixtureColor(const glm::vec3& color)
{
  auto [id, newColor] = m_mixtureColors.Insert();
  *newColor = color;

  return Draw3DPrivate::CreateMixtureColorID(id);
}

// ----------------------------------------------------------------------------

const glm::vec3*
ResourcesManager::GetMixtureColor(const MixtureColorID& colorID)
{
  const int id = Draw3DPrivate::GetMixtureColorIDValue(colorID);
  return m_mixtureColors.Find(id);
}

// ----------------------------------------------------------------------------

void
ResourcesManager::UpdateMixtureColor(const MixtureColorID& colorID,
                                     const glm::vec3& newColor)
{
  if (auto color = const_cast<glm::vec3*>(GetMixtureColor(colorID))) {
    *color = newColor;
  }
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

void
ResourcesManager::AddResourceToGroup(const std::string& tag,
                                     const MixtureColorID& mixtureColor,
                                     const ResourceGroupID& group)
{
  const unsigned idValue = Draw3DPrivate::GetResourceGroupIDValue(group);
  ResourcesGroup* groupInstance = m_resourceGroups.Find(idValue);
  if (groupInstance == nullptr) {
    Chimia::Diagnostics::Error(1, "Didn't find group instance");
  }

  groupInstance->AddResource(tag, mixtureColor);
}

// ----------------------------------------------------------------------------

const ResourcesGroup*
ResourcesManager::GetResourcesGroup(const ResourceGroupID& groupID)
{
  const unsigned idValue = Draw3DPrivate::GetResourceGroupIDValue(groupID);
  return m_resourceGroups.Find(idValue);
}

// ----------------------------------------------------------------------------
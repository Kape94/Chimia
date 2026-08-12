#include "Resources.h"

#include "ResourcesManager.h"
#include "Types.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

ModelID
CHIMIA_DRAW3D_NAMESPACE_NAME::CreateModel(const MeshDataView& meshData,
                                          const eVertexLayout vertexLayout)
{
  auto& manager = ResourcesManager::GetInstance();
  return manager.CreateModel(meshData, vertexLayout);
}

// ----------------------------------------------------------------------------

TransitionID
CHIMIA_DRAW3D_NAMESPACE_NAME::CreateTransition(const ModelID& start,
                                               const ModelID& target)
{
  auto& manager = ResourcesManager::GetInstance();
  return manager.CreateTransition(start, target);
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

TextureID
CHIMIA_DRAW3D_NAMESPACE_NAME::CreateTexture(const unsigned char* textureData,
                                            const unsigned width,
                                            const unsigned height)
{
  auto& manager = ResourcesManager::GetInstance();
  return manager.CreateTexture(textureData, width, height);
}

// ----------------------------------------------------------------------------

OpacityFactorID
CHIMIA_DRAW3D_NAMESPACE_NAME::CreateOpacityFactor(const float value)
{
  auto& manager = ResourcesManager::GetInstance();
  return manager.CreateOpacityFactor(value);
}

// -------------------------------------------------------------------------

void
CHIMIA_DRAW3D_NAMESPACE_NAME::UpdateOpacityFactor(
  const OpacityFactorID& opacityID,
  const float newValue)
{
  ResourcesManager::GetInstance().UpdateOpacityFactor(opacityID, newValue);
}

// ----------------------------------------------------------------------------

MixtureColorID
CHIMIA_DRAW3D_NAMESPACE_NAME::CreateMixtureColor(const glm::vec3& color)
{
  auto& manager = ResourcesManager::GetInstance();
  return manager.CreateMixtureColor(color);
}

// -------------------------------------------------------------------------

void
CHIMIA_DRAW3D_NAMESPACE_NAME::UpdateMixtureColor(const MixtureColorID& colorID,
                                                 const glm::vec3& newColor)
{
  ResourcesManager::GetInstance().UpdateMixtureColor(colorID, newColor);
}

// ----------------------------------------------------------------------------

ResourceGroupID
CHIMIA_DRAW3D_NAMESPACE_NAME::CreateResourceGroup()
{
  auto& manager = ResourcesManager::GetInstance();
  return manager.CreateResourceGroup();
}

// ----------------------------------------------------------------------------

void
CHIMIA_DRAW3D_NAMESPACE_NAME::AddResourceToGroup(const MaterialID& material,
                                                 const ResourceGroupID& group)
{
  auto& manager = ResourcesManager::GetInstance();
  manager.AddResourceToGroup("any", material, group);
}

// ----------------------------------------------------------------------------

void
CHIMIA_DRAW3D_NAMESPACE_NAME::AddResourceToGroup(const TextureID& texture,
                                                 const ResourceGroupID& group)
{
  auto& manager = ResourcesManager::GetInstance();
  manager.AddResourceToGroup("any", texture, group);
}

// ----------------------------------------------------------------------------

void
CHIMIA_DRAW3D_NAMESPACE_NAME::AddResourceToGroup(
  const OpacityFactorID& opacityFactor,
  const ResourceGroupID& group)
{
  auto& manager = ResourcesManager::GetInstance();
  manager.AddResourceToGroup("any", opacityFactor, group);
}

// ----------------------------------------------------------------------------

void
CHIMIA_DRAW3D_NAMESPACE_NAME::AddResourceToGroup(
  const MixtureColorID& mixtureColor,
  const ResourceGroupID& group)
{
  auto& manager = ResourcesManager::GetInstance();
  manager.AddResourceToGroup("any", mixtureColor, group);
}

// ----------------------------------------------------------------------------

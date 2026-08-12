#include "ResourceGroupHelper.h"

#include "Draw3DPrivate.h"
#include "ObjectTable.h"
#include "ResourceGroup.h"
#include "ResourcesManager.h"
#include "Types.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace ResourceGroupHelperInternal {
ObjectTable<unsigned> textureGroups;
ObjectTable<unsigned> materialGroups;

const ResourceGroupID EMPTY_RESOURCE =
  ResourcesManager::GetInstance().CreateResourceGroup();

static auto& resources = ResourcesManager::GetInstance();

const ResourcesGroup*
GetResourcesGroup(const ResourceGroupID& resourceID)
{
  return resources.GetResourcesGroup(resourceID);
}

}

// ----------------------------------------------------------------------------

ResourceGroupID
ResourceGroupHelper::GetEmptyResource()
{
  return ResourceGroupHelperInternal::EMPTY_RESOURCE;
}

// ----------------------------------------------------------------------------

ResourceGroupID
ResourceGroupHelper::WrapTexture(const TextureID& texture)
{
  using namespace ResourceGroupHelperInternal;

  const ResourceGroupID group = resources.CreateResourceGroup();
  resources.AddResourceToGroup("tex", texture, group);
  return group;
}

// ----------------------------------------------------------------------------

ResourceGroupID
ResourceGroupHelper::GetResourceGroup(const TextureID& texture)
{
  using namespace ResourceGroupHelperInternal;

  const unsigned textureIDValue = Draw3DPrivate::GetTextureIDValue(texture);
  const unsigned* groupIDValue = textureGroups.Find(textureIDValue);
  if (groupIDValue != nullptr) {
    return Draw3DPrivate::CreateResourceGroupID(*groupIDValue);
  }

  const ResourceGroupID newGroup = WrapTexture(texture);

  unsigned* newGroupIDValue = textureGroups.InsertWithID(textureIDValue);
  *newGroupIDValue = Draw3DPrivate::GetResourceGroupIDValue(newGroup);

  return newGroup;
}

// ----------------------------------------------------------------------------

ResourceGroupID
ResourceGroupHelper::WrapMaterial(const MaterialID& material)
{
  using namespace ResourceGroupHelperInternal;

  const ResourceGroupID group = resources.CreateResourceGroup();
  resources.AddResourceToGroup("default", material, group);
  return group;
}

// ----------------------------------------------------------------------------

ResourceGroupID
ResourceGroupHelper::GetResourceGroup(const MaterialID& material)
{
  using namespace ResourceGroupHelperInternal;

  const unsigned materialIDValue = Draw3DPrivate::GetMaterialIDValue(material);
  const unsigned* groupIDValue = materialGroups.Find(materialIDValue);
  if (groupIDValue != nullptr) {
    return Draw3DPrivate::CreateResourceGroupID(*groupIDValue);
  }

  const ResourceGroupID newGroup = WrapMaterial(material);

  unsigned* newGroupIDValue = materialGroups.InsertWithID(materialIDValue);
  *newGroupIDValue = Draw3DPrivate::GetResourceGroupIDValue(newGroup);

  return newGroup;
}

// ----------------------------------------------------------------------------

bool
ResourceGroupHelper::HasMaterial(const ResourceGroupID& resource)
{
  const ResourcesGroup* group =
    ResourceGroupHelperInternal::GetResourcesGroup(resource);
  return group != nullptr && group->HasMaterials();
}

// ----------------------------------------------------------------------------

bool
ResourceGroupHelper::HasTexture(const ResourceGroupID& resource)
{
  const ResourcesGroup* group =
    ResourceGroupHelperInternal::GetResourcesGroup(resource);
  return group != nullptr && group->HasTextures();
}

// ----------------------------------------------------------------------------

bool
ResourceGroupHelper::HasOpacityFactor(const ResourceGroupID& resource)
{
  const ResourcesGroup* group =
    ResourceGroupHelperInternal::GetResourcesGroup(resource);
  return group != nullptr && group->HasOpacityFactor();
}

// ----------------------------------------------------------------------------
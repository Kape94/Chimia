#include "ResourceGroupHelper.h"

#include "Draw3DPrivate.h"
#include "ObjectTable.h"
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

  unsigned* newGroupIDValue = textureGroups.Insert(textureIDValue);
  *newGroupIDValue = Draw3DPrivate::GetResourceGroupIDValue(newGroup);

  return newGroup;
}

// ----------------------------------------------------------------------------
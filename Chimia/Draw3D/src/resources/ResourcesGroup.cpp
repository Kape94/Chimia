#include "Draw3DPrivate.h"
#include "ResourceGroup.h"
#include "Types.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace {
template<typename ResourceType, typename TaggedResource>
ResourceType
FindByTag(const std::vector<TaggedResource>& resourcesWithTag,
          const std::string& tag,
          const ResourceType& defaultValue)
{
  for (const auto& taggedMaterial : resourcesWithTag) {
    if (taggedMaterial.tag == tag) {
      return taggedMaterial.resource;
    }
  }
  return defaultValue;
}
}

// ----------------------------------------------------------------------------

void
ResourcesGroup::AddResource(const std::string& tag, const MaterialID& material)
{
  m_materials.push_back({ tag, material });
}

// ----------------------------------------------------------------------------

void
ResourcesGroup::AddResource(const std::string& tag, const TextureID& texture)
{
  m_textures.push_back({ tag, texture });
}

// ----------------------------------------------------------------------------

bool
ResourcesGroup::HasMaterials() const
{
  return !m_materials.empty();
}

// ----------------------------------------------------------------------------

bool
ResourcesGroup::HasTextures() const
{
  return !m_textures.empty();
}

// ----------------------------------------------------------------------------

MaterialID
ResourcesGroup::FindMaterial(const std::string& tag) const
{
  const MaterialID defaultValue = Draw3DPrivate::CreateMaterialID(0);
  return FindByTag(m_materials, tag, defaultValue);
}

// ----------------------------------------------------------------------------

MaterialID
ResourcesGroup::FirstMaterial() const
{
  return m_materials.front().resource;
}

// ----------------------------------------------------------------------------

TextureID
ResourcesGroup::FindTexture(const std::string& tag) const
{
  const TextureID defaultValue = Draw3DPrivate::CreateTextureID(0);
  return FindByTag(m_textures, tag, defaultValue);
}

// ----------------------------------------------------------------------------

TextureID
ResourcesGroup::FirstTexture() const
{
  return m_textures.front().resource;
}

// ----------------------------------------------------------------------------
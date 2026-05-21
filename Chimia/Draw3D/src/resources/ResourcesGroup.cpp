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

void
ResourcesGroup::AddResource(const std::string& tag,
                            const OpacityFactorID& opacityFactor)
{
  m_opacityFactors.push_back({ tag, opacityFactor });
}

// ----------------------------------------------------------------------------

void
ResourcesGroup::AddResource(const std::string& tag,
                            const MixtureColorID& colorID)
{
  m_mixtureColors.push_back({ tag, colorID });
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

bool
ResourcesGroup::HasOpacityFactor() const
{
  return !m_opacityFactors.empty();
}

// ----------------------------------------------------------------------------

bool
ResourcesGroup::HasMixtureColor() const
{
  return !m_mixtureColors.empty();
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

OpacityFactorID
ResourcesGroup::FindOpacityFactor(const std::string& tag) const
{
  const OpacityFactorID defaultValue = Draw3DPrivate::CreateOpacityFactorID(0);
  return FindByTag(m_opacityFactors, tag, defaultValue);
}

// ----------------------------------------------------------------------------

OpacityFactorID
ResourcesGroup::FirstOpacityFactor() const
{
  return m_opacityFactors.front().resource;
}

// ----------------------------------------------------------------------------

MixtureColorID
ResourcesGroup::FindMixtureColor(const std::string& tag) const
{
  const MixtureColorID defaultValue = Draw3DPrivate::CreateMixtureColorID(0);
  return FindByTag(m_mixtureColors, tag, defaultValue);
}

// ----------------------------------------------------------------------------

MixtureColorID
ResourcesGroup::FirstMixtureColor() const
{
  return m_mixtureColors.front().resource;
}

// ----------------------------------------------------------------------------
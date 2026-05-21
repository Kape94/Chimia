#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "Types.h"

#include "Core/ClassDefs.h"

#include <string>
#include <vector>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class ResourcesGroup
{
public:
  DEFAULT_CONSTUCTIBLE(ResourcesGroup);
  NON_COPYABLE_NON_MOVABLE(ResourcesGroup);

  void AddResource(const std::string& tag, const MaterialID& material);
  void AddResource(const std::string& tag, const TextureID& texture);
  void AddResource(const std::string& tag,
                   const OpacityFactorID& opacityFactor);
  void AddResource(const std::string& tag, const MixtureColorID& mixtureColor);

  bool HasMaterials() const;
  bool HasTextures() const;
  bool HasOpacityFactor() const;
  bool HasMixtureColor() const;

  MaterialID FindMaterial(const std::string& tag) const;
  MaterialID FirstMaterial() const;

  TextureID FindTexture(const std::string& tag) const;
  TextureID FirstTexture() const;

  OpacityFactorID FindOpacityFactor(const std::string& tag) const;
  OpacityFactorID FirstOpacityFactor() const;

  MixtureColorID FindMixtureColor(const std::string& tag) const;
  MixtureColorID FirstMixtureColor() const;

private:
  template<typename ResourceType>
  struct TaggedResource
  {
    std::string tag;
    ResourceType resource;
  };

  std::vector<TaggedResource<MaterialID>> m_materials;
  std::vector<TaggedResource<TextureID>> m_textures;
  std::vector<TaggedResource<OpacityFactorID>> m_opacityFactors;
  std::vector<TaggedResource<MixtureColorID>> m_mixtureColors;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
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

  bool HasMaterials() const;
  bool HasTextures() const;

  MaterialID FindMaterial(const std::string& tag) const;
  MaterialID FirstMaterial() const;

  TextureID FindTexture(const std::string& tag) const;
  TextureID FirstTexture() const;

private:
  template<typename ResourceType>
  struct TaggedResource
  {
    std::string tag;
    ResourceType resource;
  };

  std::vector<TaggedResource<MaterialID>> m_materials;
  std::vector<TaggedResource<TextureID>> m_textures;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
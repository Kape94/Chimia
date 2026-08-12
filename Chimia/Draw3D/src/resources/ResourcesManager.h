#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Draw3DNamespaceDefs.h"

#include "InternalTypes.h"
#include "Model.h"
#include "ObjectTable.h"
#include "Rendering/Texture2D.h"
#include "Transition.h"
#include "Types.h"
#include "resources/ResourceGroup.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class ResourcesManager
{
public:
  static ResourcesManager& GetInstance();

  ModelID CreateModel(const MeshDataView& meshDataconst,
                      const eVertexLayout vertexLayout);

  const Model* GetModel(const ModelID& modelID) const;

  TransitionID CreateTransition(const ModelID& start, const ModelID& target);

  const Transition* GetTransition(const TransitionID& transitionID) const;

  MaterialID CreateMaterial(const glm::vec3& ambient,
                            const glm::vec3& diffuse,
                            const glm::vec3& specular,
                            const float shininess);

  const Material* GetMaterial(const MaterialID& materialID);

  TextureID CreateTexture(const unsigned char* textureData,
                          const unsigned width,
                          const unsigned height);

  const Chimia::Rendering::Texture2DInstance* GetTexture(
    const TextureID& textureID);

  OpacityFactorID CreateOpacityFactor(const float value);

  const float* GetOpacityFactor(const OpacityFactorID& opacityFactorID);

  void UpdateOpacityFactor(const OpacityFactorID& opacityID,
                           const float newValue);

  MixtureColorID CreateMixtureColor(const glm::vec3& color);

  const glm::vec3* GetMixtureColor(const MixtureColorID& colorID);

  void UpdateMixtureColor(const MixtureColorID& colorID,
                          const glm::vec3& newColor);

  ResourceGroupID CreateResourceGroup();

  void AddResourceToGroup(const std::string& tag,
                          const MaterialID& material,
                          const ResourceGroupID& group);
  void AddResourceToGroup(const std::string& tag,
                          const TextureID& texture,
                          const ResourceGroupID& group);
  void AddResourceToGroup(const std::string& tag,
                          const OpacityFactorID& opacityFactor,
                          const ResourceGroupID& group);
  void AddResourceToGroup(const std::string& tag,
                          const MixtureColorID& mixtureColor,
                          const ResourceGroupID& group);

  const ResourcesGroup* GetResourcesGroup(const ResourceGroupID& groupID);

private:
  DEFAULT_CONSTUCTIBLE(ResourcesManager)
  NON_COPYABLE_NON_MOVABLE(ResourcesManager)

  ObjectTable<Model> m_modelsTable;

  ObjectTable<Transition> m_transitionsTable;

  ObjectTable<Material> m_materialsTable;

  ObjectTable<Rendering::Texture2DInstance> m_texturesTable;

  ObjectTable<float> m_opacityFactors;

  ObjectTable<glm::vec3> m_mixtureColors;

  ObjectTable<ResourcesGroup> m_resourceGroups;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
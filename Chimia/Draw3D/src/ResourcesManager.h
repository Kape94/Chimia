#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Draw3DNamespaceDefs.h"

#include "InternalTypes.h"
#include "Model.h"
#include "ObjectTable.h"
#include "Rendering/Texture2D.h"
#include "Types.h"

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

  MaterialID CreateMaterial(const glm::vec3& ambient,
                            const glm::vec3& diffuse,
                            const glm::vec3& specular,
                            const float shininess);

  const Material* GetMaterial(const MaterialID& materialID);

  TextureID CreateTexture(const unsigned char* textureData,
                          const unsigned width,
                          const unsigned height);

  Rendering::Texture2D* GetTexture(const TextureID& textureID);

private:
  DEFAULT_CONSTUCTIBLE(ResourcesManager)
  NON_COPYABLE_NON_MOVABLE(ResourcesManager)

  ObjectTable<Model> m_modelsTable;

  ObjectTable<Material> m_materialsTable;

  ObjectTable<Rendering::Texture2D> m_texturesTable;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
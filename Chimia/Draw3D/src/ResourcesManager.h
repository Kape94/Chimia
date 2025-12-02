#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include "Model.h"
#include "ObjectTable.h"
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

private:
  ResourcesManager() = default;

  ObjectTable<Model> m_modelsTable;

  ObjectTable<Material> m_materialsTable;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
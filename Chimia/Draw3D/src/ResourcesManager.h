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

  ModelID CreateModel(const MeshDataView& meshData);

  const Model* GetModel(const ModelID& modelID) const;

private:
  ResourcesManager() = default;

  ObjectTable<Model> m_modelsTable;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
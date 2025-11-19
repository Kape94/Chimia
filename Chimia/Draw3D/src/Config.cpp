#include "Config.h"
#include "Draw3DNamespaceDefs.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace ConfigInternal {

namespace VertexColored {
size_t triangleBatchSize = 100;
size_t modelsBatchSize = 2;
}

namespace Lit {
size_t triangleBatchSizePerMaterial = 100;
size_t modelsBatchSize = 2;
eIlluminationModel illuminationModel = eIlluminationModel::GOURAUD;
}
};

// ----------------------------------------------------------------------------
// VertexColored
// ----------------------------------------------------------------------------

void
Config::VertexColored::SetTriangleBatchSize(const size_t batchSize)
{
  ConfigInternal::VertexColored::triangleBatchSize = batchSize;
}

size_t
Config::VertexColored::TriangleBatchSize()
{
  return ConfigInternal::VertexColored::triangleBatchSize;
}

// ----------------------------------------------------------------------------

void
Config::VertexColored::SetModelsBatchSize(const size_t batchSize)
{
  ConfigInternal::VertexColored::modelsBatchSize = batchSize;
}

size_t
Config::VertexColored::ModelsBatchSize()
{
  return ConfigInternal::VertexColored::modelsBatchSize;
}

// ----------------------------------------------------------------------------
// Lit
// ----------------------------------------------------------------------------

void
Config::Lit::SetTriangleBatchSizePerMaterial(const size_t batchSize)
{
  ConfigInternal::Lit::triangleBatchSizePerMaterial = batchSize;
}

size_t
Config::Lit::TriangleBatchSizePerMaterial()
{
  return ConfigInternal::Lit::triangleBatchSizePerMaterial;
}

// ----------------------------------------------------------------------------

void
Config::Lit::SetModelsBatchSize(const size_t batchSize)
{
  ConfigInternal::Lit::modelsBatchSize = batchSize;
}

size_t
Config::Lit::ModelsBatchSize()
{
  return ConfigInternal::Lit::modelsBatchSize;
}

// ----------------------------------------------------------------------------

eIlluminationModel
Config::Lit::IlluminationModel()
{
  return ConfigInternal::Lit::illuminationModel;
}

void
Config::Lit::SetIlluminationModel(eIlluminationModel model)
{
  ConfigInternal::Lit::illuminationModel = model;
}

// ----------------------------------------------------------------------------
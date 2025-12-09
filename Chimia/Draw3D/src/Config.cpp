#include "Config.h"
#include "Types.h"

#include <limits>

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace ConfigInternal {

constexpr size_t UNLIMITED = std::numeric_limits<size_t>::max();

namespace Batching {
BatchingSettings triangleBatching{ 50, UNLIMITED };

BatchingSettings modelBatching{ 10, UNLIMITED };

BatchingSettings triangleBatchingByResource{ 20, UNLIMITED };

BatchingSettings modelBatchingByResource{ 2, UNLIMITED };
}

eIlluminationModel illuminationModel = eIlluminationModel::GOURAUD;
};

// ----------------------------------------------------------------------------
// Batching
// ----------------------------------------------------------------------------

BatchingSettings&
Config::Batching::TriangleBatchingSettings()
{
  return ConfigInternal::Batching::triangleBatching;
}

BatchingSettings&
Config::Batching::TriangleBatchingByResourceSettings()
{
  return ConfigInternal::Batching::triangleBatchingByResource;
}

BatchingSettings&
Config::Batching::ModelBatchingSettings()
{
  return ConfigInternal::Batching::modelBatching;
}

BatchingSettings&
Config::Batching::ModelBatchingByResourceSettings()
{
  return ConfigInternal::Batching::modelBatchingByResource;
}

// ----------------------------------------------------------------------------

eIlluminationModel
Config::IlluminationModel()
{
  return ConfigInternal::illuminationModel;
}

void
Config::SetIlluminationModel(eIlluminationModel model)
{
  ConfigInternal::illuminationModel = model;
}

// ----------------------------------------------------------------------------
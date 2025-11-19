#pragma once

#include "Draw3DNamespaceDefs.h"
#include "Types.h"

#include <cstddef>

BEGIN_CHIMIA_DRAW3D_NAMESPACE

namespace Config {

namespace VertexColored {

void
SetTriangleBatchSize(const size_t batchSize);
size_t
TriangleBatchSize();

void
SetModelsBatchSize(const size_t batchSize);
size_t
ModelsBatchSize();
}

namespace Lit {
void
SetTriangleBatchSizePerMaterial(const size_t batchSize);
size_t
TriangleBatchSizePerMaterial();

void
SetModelsBatchSize(const size_t batchSize);
size_t
ModelsBatchSize();

eIlluminationModel
IlluminationModel();
void
SetIlluminationModel(eIlluminationModel model);
}

}

END_CHIMIA_DRAW3D_NAMESPACE
#pragma once

#include "Draw3DNamespaceDefs.h"

#include <cstddef>

BEGIN_CHIMIA_DRAW3D_NAMESPACE

namespace Config {

namespace VertexColored {
static size_t triangleBatchSize = 100;
static size_t modelsBatchSize = 2;
static size_t indexedTrianglesVertexBatchSize = 200;
static size_t indexedTrianglesIndexBatchSize = 500;
}

namespace GouraudLit {
static size_t triangleBatchSizePerMaterial = 100;
static size_t modelsBatchSize = 2;
}

}

END_CHIMIA_DRAW3D_NAMESPACE
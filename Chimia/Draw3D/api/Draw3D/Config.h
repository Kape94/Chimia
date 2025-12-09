#pragma once

#include "Draw3DNamespaceDefs.h"
#include "Types.h"

BEGIN_CHIMIA_DRAW3D_NAMESPACE

namespace Config {

/*
    Using batching settings we can specify control attributes like initial
    and maximum sizes for the batches.

    --------------------------------------------------------------------------

    The size for triangle batches is specified in number of triangles
    (e.g. a batch size of 100 means that we have room for 100 triangles in the
   batch). The size in bytes varies depending on the vertex layout used for
   the triangles. For example, a triangle composed of vertices with
   position (3D), color (RGB) and normal (3D) will consume:

   float size = 4 bytes;
   position size = 3 * 4 (3 floats of 4 bytes) = 12 bytes
   color size = 3 * 4 = 12 bytes
   normal size = 3 * 4 = 12 bytes

   vertex size = position size + color size + normal size;
   vertex size = 12 + 12 + 12 = 36 bytes;

   triangle size = 3 * vertex size (triangle has 3 vertices)
   triangle size = 3 * 36 = 108 bytes

   --------------------------------------------------------------------------

   For models, the renderers do instanced rendering, so the batches
   contain only per-instance data.

   The size for model batches is specified in terms of per-instance data size.
   The data used for each instance of a model typically is a 4x4 matrix, that
   stores the transform for that model. So, given that, the size for a single
   instance of a model is usually:

   float size = 4 bytes
   mat4x4 size = 16 floats = 16 * 4 = 64 bytes

   --------------------------------------------------------------------------

   There are batches that are created by demand for a resource. A resource
   refers to some additional property to the model/triangle being drawn,
   like a material or texture.

   The renderer creates separated batches for each resource, so the settings
   that mention "ByResource" in their name refer to these batches created
   by demand.

   You may want to use lower initial values for these batches, to avoid
   allocating unnecessarily big batches for few drawings.

   --------------------------------------------------------------------------

*/
namespace Batching {
BatchingSettings&
TriangleBatchingSettings();
BatchingSettings&
TriangleBatchingByResourceSettings();
BatchingSettings&
ModelBatchingSettings();
BatchingSettings&
ModelBatchingByResourceSettings();
}

eIlluminationModel
IlluminationModel();
void
SetIlluminationModel(eIlluminationModel model);

}

END_CHIMIA_DRAW3D_NAMESPACE
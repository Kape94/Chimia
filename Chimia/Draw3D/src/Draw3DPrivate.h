#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include "Types.h"

#include <utility>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class Draw3DPrivate
{
public:
  static ModelID CreateModelID(unsigned id);
  static unsigned GetModelID(const ModelID& modelID);

  static ModelInstanceID CreateModelInstanceID(unsigned modelID,
                                               unsigned instanceID);
  static std::pair<unsigned, unsigned> GetModelInstanceIDs(
    const ModelInstanceID& instanceID);

  static TriangleMeshID CreateTriangleMeshID(unsigned id);
  static unsigned GetTriangleMeshIDValue(const TriangleMeshID& meshID);

  static MaterialID CreateMaterialID(unsigned id);
  static unsigned GetMaterialIDValue(const MaterialID& materialID);

  static LitTriangleMeshID CreateLitTriangleMeshID(unsigned id,
                                                   unsigned materialID);
  static std::pair<unsigned, unsigned> GetLitTriangleMeshIDValues(
    const LitTriangleMeshID& meshID);
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
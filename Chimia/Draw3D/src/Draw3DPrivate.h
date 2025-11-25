#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include "InternalTypes.h"
#include "Types.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class Draw3DPrivate
{
public:
  // ModelID
  static ModelID CreateModelID(unsigned id);
  static unsigned GetModelID(const ModelID& modelID);

  // ModelInstanceID
  static ModelInstanceID CreateModelInstanceID(unsigned rendererID,
                                               unsigned modelID,
                                               unsigned instanceID);
  static ModelInstanceID CreateModelInstanceID(
    unsigned rendererID,
    const LocalModelInstanceID& localInstanceID);
  static std::tuple<unsigned, unsigned, unsigned> GetModelInstanceIDValues(
    const ModelInstanceID& instanceID);

  // LocalModelInstanceID
  static LocalModelInstanceID CreateLocalModelInstanceID(unsigned modelID,
                                                         unsigned instanceID);
  static LocalModelInstanceID CreateLocalModelInstanceID(
    const ModelInstanceID& globalInstanceID);
  static std::pair<unsigned, unsigned> GetLocalModelInstanceIDValues(
    const LocalModelInstanceID& instanceID);

  // MaterialID
  static MaterialID CreateMaterialID(unsigned id);
  static unsigned GetMaterialIDValue(const MaterialID& materialID);

  // TriangleMeshID
  static TriangleMeshID CreateTriangleMeshID(unsigned rendererID,
                                             unsigned id,
                                             unsigned materialID);
  static std::tuple<unsigned, unsigned, unsigned> GetTriangleMeshIDValues(
    const TriangleMeshID& meshID);
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
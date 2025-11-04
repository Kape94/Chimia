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
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
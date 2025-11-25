#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class LocalModelInstanceID
{
private:
  friend class Draw3DPrivate;

  LocalModelInstanceID() = delete;
  LocalModelInstanceID(unsigned modelID, unsigned instanceID)
    : m_modelID(modelID)
    , m_instanceID(instanceID)
  {
  }

  unsigned m_modelID = 0;
  unsigned m_instanceID = 0;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
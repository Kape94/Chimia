#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class ModelID
{
private:
  friend class Draw3DPrivate;

  ModelID() = delete;
  ModelID(unsigned id)
    : m_id(id)
  {
  }

  unsigned m_id = 0;
};

// ----------------------------------------------------------------------------

class ModelInstanceID
{
private:
  friend class Draw3DPrivate;

  ModelInstanceID() = delete;
  ModelInstanceID(unsigned modelID, unsigned instanceID)
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
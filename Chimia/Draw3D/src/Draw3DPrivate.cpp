#include "Draw3DPrivate.h"
#include "Types.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

ModelID
Draw3DPrivate::CreateModelID(unsigned id)
{
  return ModelID(id);
}

// ----------------------------------------------------------------------------

unsigned
Draw3DPrivate::GetModelID(const ModelID& modelID)
{
  return modelID.m_id;
}

// ----------------------------------------------------------------------------

ModelInstanceID
Draw3DPrivate::CreateModelInstanceID(unsigned modelID, unsigned instanceID)
{
  return ModelInstanceID(modelID, instanceID);
}

// ----------------------------------------------------------------------------

std::pair<unsigned, unsigned>
Draw3DPrivate::GetModelInstanceIDs(const ModelInstanceID& instanceID)
{
  return { instanceID.m_modelID, instanceID.m_instanceID };
}

// ----------------------------------------------------------------------------
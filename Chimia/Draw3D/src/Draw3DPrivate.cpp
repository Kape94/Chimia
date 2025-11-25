#include "Draw3DPrivate.h"
#include "InternalTypes.h"
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
Draw3DPrivate::CreateModelInstanceID(unsigned rendererID,
                                     unsigned modelID,
                                     unsigned instanceID)
{
  return ModelInstanceID(rendererID, modelID, instanceID);
}

// ----------------------------------------------------------------------------

ModelInstanceID
Draw3DPrivate::CreateModelInstanceID(
  unsigned rendererID,
  const LocalModelInstanceID& localInstanceID)
{
  return ModelInstanceID(
    rendererID, localInstanceID.m_modelID, localInstanceID.m_instanceID);
}

// ----------------------------------------------------------------------------

std::tuple<unsigned, unsigned, unsigned>
Draw3DPrivate::GetModelInstanceIDValues(const ModelInstanceID& instanceID)
{
  return { instanceID.m_rendererID,
           instanceID.m_modelID,
           instanceID.m_instanceID };
}

// ----------------------------------------------------------------------------

LocalModelInstanceID
Draw3DPrivate::CreateLocalModelInstanceID(unsigned modelID, unsigned instanceID)
{
  return LocalModelInstanceID(modelID, instanceID);
}

// ----------------------------------------------------------------------------

LocalModelInstanceID
Draw3DPrivate::CreateLocalModelInstanceID(
  const ModelInstanceID& globalInstanceID)
{
  return LocalModelInstanceID(globalInstanceID.m_modelID,
                              globalInstanceID.m_instanceID);
}

// ----------------------------------------------------------------------------

std::pair<unsigned, unsigned>
Draw3DPrivate::GetLocalModelInstanceIDValues(
  const LocalModelInstanceID& instanceID)
{
  return { instanceID.m_modelID, instanceID.m_instanceID };
}

// ----------------------------------------------------------------------------

MaterialID
Draw3DPrivate::CreateMaterialID(unsigned id)
{
  return MaterialID(id);
}

// ----------------------------------------------------------------------------

unsigned
Draw3DPrivate::GetMaterialIDValue(const MaterialID& materialID)
{
  return materialID.m_id;
}

// ----------------------------------------------------------------------------

TriangleMeshID
Draw3DPrivate::CreateTriangleMeshID(unsigned rendererID,
                                    unsigned id,
                                    unsigned materialID)
{
  return TriangleMeshID(rendererID, id, materialID);
}

// ----------------------------------------------------------------------------

std::tuple<unsigned, unsigned, unsigned>
Draw3DPrivate::GetTriangleMeshIDValues(const TriangleMeshID& meshID)
{
  return { meshID.m_rendererID, meshID.m_id, meshID.m_materialID };
}

// ----------------------------------------------------------------------------

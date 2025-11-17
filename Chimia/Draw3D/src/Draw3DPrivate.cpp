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

TriangleMeshID
Draw3DPrivate::CreateTriangleMeshID(unsigned id)
{
  return TriangleMeshID(id);
}

// ----------------------------------------------------------------------------

unsigned
Draw3DPrivate::GetTriangleMeshIDValue(const TriangleMeshID& meshID)
{
  return meshID.m_id;
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

LitTriangleMeshID
Draw3DPrivate::CreateLitTriangleMeshID(unsigned id, unsigned materialID)
{
  return LitTriangleMeshID(id, materialID);
}

// ----------------------------------------------------------------------------

std::pair<unsigned, unsigned>
Draw3DPrivate::GetLitTriangleMeshIDValues(const LitTriangleMeshID& meshID)
{
  return { meshID.m_id, meshID.m_materialID };
}

// ----------------------------------------------------------------------------
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
                                     unsigned instanceID,
                                     unsigned resourceID)
{
  return ModelInstanceID(rendererID, modelID, instanceID, resourceID);
}

// ----------------------------------------------------------------------------

ModelInstanceID
Draw3DPrivate::CreateModelInstanceID(
  unsigned rendererID,
  const LocalModelInstanceID& localInstanceID,
  unsigned resourceID)
{
  return ModelInstanceID(rendererID,
                         localInstanceID.m_modelID,
                         localInstanceID.m_instanceID,
                         resourceID);
}

// ----------------------------------------------------------------------------

std::tuple<unsigned, unsigned, unsigned, unsigned>
Draw3DPrivate::GetModelInstanceIDValues(const ModelInstanceID& instanceID)
{
  return { instanceID.m_rendererID,
           instanceID.m_modelID,
           instanceID.m_instanceID,
           instanceID.m_resourceID };
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

TransitionID
Draw3DPrivate::CreateTransitionID(unsigned id)
{
  return TransitionID(id);
}

// ----------------------------------------------------------------------------

unsigned
Draw3DPrivate::GetTransitionIDValue(const TransitionID& transitionID)
{
  return transitionID.m_id;
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

TextureID
Draw3DPrivate::CreateTextureID(unsigned id)
{
  return TextureID(id);
}

// ----------------------------------------------------------------------------

unsigned
Draw3DPrivate::GetTextureIDValue(const TextureID& textureID)
{
  return textureID.m_id;
}

// ----------------------------------------------------------------------------

OpacityFactorID
Draw3DPrivate::CreateOpacityFactorID(unsigned id)
{
  return OpacityFactorID(id);
}

// ----------------------------------------------------------------------------

unsigned
Draw3DPrivate::GetOpacityFactorIDValue(const OpacityFactorID& opacityID)
{
  return opacityID.m_id;
}

// ----------------------------------------------------------------------------

MixtureColorID
Draw3DPrivate::CreateMixtureColorID(unsigned id)
{
  return MixtureColorID(id);
}

// ----------------------------------------------------------------------------

unsigned
Draw3DPrivate::GetMixtureColorIDValue(const MixtureColorID& colorID)
{
  return colorID.m_id;
}

// ----------------------------------------------------------------------------

ResourceGroupID
Draw3DPrivate::CreateResourceGroupID(unsigned id)
{
  return ResourceGroupID(id);
}

// ----------------------------------------------------------------------------

unsigned
Draw3DPrivate::GetResourceGroupIDValue(const ResourceGroupID& resourceGroupID)
{
  return resourceGroupID.m_id;
}

// ----------------------------------------------------------------------------

TriangleMeshID
Draw3DPrivate::CreateTriangleMeshID(unsigned rendererID,
                                    unsigned id,
                                    unsigned resourceID)
{
  return TriangleMeshID(rendererID, id, resourceID);
}

// ----------------------------------------------------------------------------

std::tuple<unsigned, unsigned, unsigned>
Draw3DPrivate::GetTriangleMeshIDValues(const TriangleMeshID& meshID)
{
  return { meshID.m_rendererID, meshID.m_id, meshID.m_resourceID };
}

// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Draw3DNamespaceDefs.h"

#include "InternalTypes.h"
#include "Types.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class Draw3DPrivate
{
public:
  STATIC_CLASS(Draw3DPrivate)

  // ModelID
  static ModelID CreateModelID(unsigned id);
  static unsigned GetModelID(const ModelID& modelID);

  // ModelInstanceID
  static ModelInstanceID CreateModelInstanceID(unsigned rendererID,
                                               unsigned modelID,
                                               unsigned instanceID,
                                               unsigned resourceID);
  static ModelInstanceID CreateModelInstanceID(
    unsigned rendererID,
    const LocalModelInstanceID& localInstanceID,
    unsigned resourceID);
  static std::tuple<unsigned, unsigned, unsigned, unsigned>
  GetModelInstanceIDValues(const ModelInstanceID& instanceID);

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

  // TextureID
  static TextureID CreateTextureID(unsigned id);
  static unsigned GetTextureIDValue(const TextureID& textureID);

  // OpacityFactorID
  static OpacityFactorID CreateOpacityFactorID(unsigned id);
  static unsigned GetOpacityFactorIDValue(const OpacityFactorID& opacityID);

  // MixtureColorID
  static MixtureColorID CreateMixtureColorID(unsigned id);
  static unsigned GetMixtureColorIDValue(const MixtureColorID& colorID);

  // ResourceGroupID
  static ResourceGroupID CreateResourceGroupID(unsigned id);
  static unsigned GetResourceGroupIDValue(const ResourceGroupID& textureID);

  // TriangleMeshID
  static TriangleMeshID CreateTriangleMeshID(unsigned rendererID,
                                             unsigned id,
                                             unsigned resourceID);
  static std::tuple<unsigned, unsigned, unsigned> GetTriangleMeshIDValues(
    const TriangleMeshID& meshID);
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
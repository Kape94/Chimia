#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "DataBindingProvider.h"
#include "Draw3DNamespaceDefs.h"

#include "Core/ObjectRegistry.h"
#include "ImmediateModelInstancesBatch.h"
#include "InternalTypes.h"
#include "RetainedModelInstancesBatch.h"
#include "Types.h"

#include <glm/ext/matrix_float4x4.hpp>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class ModelRenderingComponent
{
public:
  DEFAULT_CONSTUCTIBLE(ModelRenderingComponent)
  NON_COPYABLE_NON_MOVABLE(ModelRenderingComponent)

  void Init(const BatchingSettings& batchingSettings,
            const DataBindingProvider& dataBindings,
            const std::function<void(void)>& onFlush);

  void Flush(const eImmediateFlusingPolicy flushingPolicy);

  void DrawModel(const ModelID& modelID, const RawDataView& instanceData);
  void DrawModel(const ModelID& modelID,
                 const std::initializer_list<RawDataView>& instanceDatas);

  LocalModelInstanceID AddRetainedModel(const ModelID& modelID,
                                        const RawDataView& instanceData);
  LocalModelInstanceID AddRetainedModel(
    const ModelID& modelID,
    const std::initializer_list<RawDataView>& instanceDatas);

  void DeleteRetainedModel(const LocalModelInstanceID& instanceID);

private:
  ImmediateModelInstancesBatch* AllocateImmediateBatchForModelInstances(
    const ModelID& modelID);

  RetainedModelInstancesBatch* AllocateRetainedBatchForModelInstances(
    const ModelID& modelID);

  BatchingSettings m_batchingSettings;
  DataBindingProvider m_dataBindingProvider;
  std::function<void(void)> m_onFlush;

  ObjectRegistry<ImmediateModelInstancesBatch> m_transformedModelsTable;
  ObjectRegistry<RetainedModelInstancesBatch> m_staticModelsTable;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
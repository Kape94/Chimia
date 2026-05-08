#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Draw3DNamespaceDefs.h"

#include "ImmediateModelInstancesBatch.h"
#include "InternalTypes.h"
#include "ObjectTable.h"
#include "RetainedModelInstancesBatch.h"
#include "Types.h"

#include "Rendering/ShaderAttribute.h"

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
            const Rendering::ShaderAttributes& vertexAttributes,
            const Rendering::ShaderAttributes& instanceAttributes,
            const std::function<void(void)>& onFlush);

  void Flush();

  void DrawModel(const ModelID& modelID, const RawDataView& instanceData);
  void DrawModel(const ModelID& modelID,
                 const std::initializer_list<RawDataView>& instanceDatas);

  LocalModelInstanceID AddStaticModel(const ModelID& modelID,
                                      const RawDataView& instanceData);
  LocalModelInstanceID AddStaticModel(
    const ModelID& modelID,
    const std::initializer_list<RawDataView>& instanceDatas);

  void DeleteStaticModel(const LocalModelInstanceID& instanceID);

private:
  ImmediateModelInstancesBatch* AllocateBatchForModelDrawing(
    const ModelID& modelID);

  RetainedModelInstancesBatch* AllocateBatchForStaticModel(
    const ModelID& modelID);

  BatchingSettings m_batchingSettings;
  Rendering::ShaderAttributes m_vertexAttributes;
  Rendering::ShaderAttributes m_instanceAttributes;
  std::function<void(void)> m_onFlush;

  ObjectTable<ImmediateModelInstancesBatch> m_transformedModelsTable;
  ObjectTable<RetainedModelInstancesBatch> m_staticModelsTable;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
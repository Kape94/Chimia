#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include "Model.h"
#include "ModelBatch.h"
#include "ObjectTable.h"
#include "StaticModel.h"
#include "Types.h"

#include "Rendering/ShaderAttribute.h"

#include <glm/ext/matrix_float4x4.hpp>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class ModelRenderingComponent
{
public:
  void Init(const size_t modelBatchSize,
            const Rendering::ShaderAttributes& vertexAttributes,
            const Rendering::ShaderAttributes& instanceAttributes,
            const std::function<void(void)>& onFlush);

  void Flush();

  void DrawModel(const ModelID& modelID, const RawDataView& instanceData);
  void DrawModel(const ModelID& modelID,
                 const std::initializer_list<RawDataView>& instanceDatas);

  ModelInstanceID AddStaticModel(const ModelID& modelID,
                                 const RawDataView& instanceData);
  ModelInstanceID AddStaticModel(
    const ModelID& modelID,
    const std::initializer_list<RawDataView>& instanceDatas);

  void DeleteStaticModel(const ModelInstanceID& instanceID);

private:
  ModelBatch* AllocateBatchForModelDrawing(const ModelID& modelID);

  StaticModel* AllocateBatchForStaticModel(const ModelID& modelID);

  size_t m_batchSize;
  Rendering::ShaderAttributes m_vertexAttributes;
  Rendering::ShaderAttributes m_instanceAttributes;
  std::function<void(void)> m_onFlush;

  ObjectTable<Model> m_modelsTable;
  ObjectTable<ModelBatch> m_transformedModelsTable;
  ObjectTable<StaticModel> m_staticModelsTable;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
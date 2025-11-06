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
  void Init();

  void Flush();

  ModelID CreateModel(const std::vector<float>& vertexData,
                      const std::vector<unsigned>& indices,
                      const size_t modelBatchSize,
                      const Rendering::ShaderAttributes& vertexAttributes,
                      const Rendering::ShaderAttributes& instanceAttributes,
                      const std::function<void(void)>& onFlush);

  void DrawModel(const ModelID& modelID, const Bits::RawDataView& instanceData);

  ModelInstanceID AddStaticModel(const ModelID& modelID,
                                 const Bits::RawDataView& instanceData);

  void DeleteStaticModel(const ModelInstanceID& instanceID);

private:
  ObjectTable<Model> m_modelsTable;
  ObjectTable<ModelBatch> m_transformedModelsTable;
  ObjectTable<StaticModel> m_staticModelsTable;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
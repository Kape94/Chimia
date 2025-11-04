#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "IndexedTriangleBatch.h"
#include "Model.h"
#include "ModelBatch.h"
#include "ObjectTable.h"
#include "Renderers.h"
#include "StaticModel.h"
#include "StaticTriangles.h"
#include "TriangleBatch.h"
#include "Types.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

class VertexColoredRendererImpl : public VertexColoredRenderer
{
public:
  static VertexColoredRendererImpl& getInstance();

  void Init();

  void Flush();

  void DrawTriangle(const glm::vec3& p1,
                    const glm::vec3& color1,
                    const glm::vec3& p2,
                    const glm::vec3& color2,
                    const glm::vec3& p3,
                    const glm::vec3& color3) override;

  void DrawTriangles(const std::vector<float>& vertexData) override;

  unsigned AddStaticTriangles(const std::vector<float>& vertexData) override;

  void DeleteStaticTriangles(unsigned id) override;

  ModelID CreateModel(const std::vector<float>& vertexData,
                      const std::vector<unsigned>& indices) override;

  void DrawModelTransformed(const ModelID& modelID,
                            const glm::mat4x4& transform) override;

  ModelInstanceID AddStaticModel(const ModelID& modelID,
                                 const glm::mat4x4& transform) override;

  void DeleteStaticModel(const ModelInstanceID& instanceID) override;

  void DrawIndexedTriangles(const std::vector<float>& vertexData,
                            const std::vector<unsigned>& indexData) override;

private:
  void ConfigureShaderForTriangleDrawing();
  void ConfigureShaderForTransformedModelDrawing();

  VertexColoredRendererImpl() = default;

  VertexColoredRendererImpl(const VertexColoredRendererImpl& other) = delete;
  VertexColoredRendererImpl& operator=(const VertexColoredRendererImpl& other) =
    delete;

  VertexColoredRendererImpl(VertexColoredRendererImpl&& other) = delete;
  VertexColoredRendererImpl& operator=(VertexColoredRendererImpl&& other) =
    delete;

  TriangleBatch m_triangleBatch;
  IndexedTriangleBatch m_indexedTriangleBatch;

  // IdentifiedObjectTable<Model> m_modelsTable;
  // std::map<unsigned, ModelBatch> m_transformedModelsTable;
  ObjectTable<Model> m_modelsTable;
  ObjectTable<ModelBatch> m_transformedModelsTable;
  ObjectTable<StaticModel> m_staticModelsTable;

  // IdentifiedObjectTable<StaticTriangles> m_staticTriangles;
  ObjectTable<StaticTriangles> m_staticTriangles;
};

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
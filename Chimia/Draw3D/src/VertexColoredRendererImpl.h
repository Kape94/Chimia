#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "IdentifiedObjectTable.h"
#include "IndexedTriangleBatch.h"
#include "ModelBatch.h"
#include "Renderers.h"
#include "TriangleBatch.h"

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

  unsigned CreateModel(const std::vector<float>& vertexData,
                       const std::vector<unsigned>& indices) override;

  void DrawModelTransformed(unsigned modelID,
                            const glm::mat4x4& transform) override;

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
  IdentifiedObjectTable<ModelBatch> m_transformedModelsTable;
};

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
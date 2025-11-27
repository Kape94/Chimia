#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "ModelRenderingComponent.h"
#include "Renderers.h"
#include "TriangleMeshComponent.h"
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

  void DrawTriangles(const std::vector<float>& vertexData,
                     const std::vector<unsigned>& indexData) override;

  TriangleMeshID AddStaticTriangles(
    const std::vector<float>& vertexData) override;

  TriangleMeshID AddStaticTriangles(
    const std::vector<float>& vertexData,
    const std::vector<unsigned>& indexData) override;

  void DeleteStaticTriangles(const TriangleMeshID& meshID) override;

  void DrawModelTransformed(const ModelID& modelID,
                            const glm::mat4x4& transform) override;

  ModelInstanceID AddStaticModel(const ModelID& modelID,
                                 const glm::mat4x4& transform) override;

  void DeleteStaticModel(const ModelInstanceID& instanceID) override;

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

  TriangleMeshComponent m_triangleMeshComponent;
  ModelRenderingComponent m_modelComponent;
};

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
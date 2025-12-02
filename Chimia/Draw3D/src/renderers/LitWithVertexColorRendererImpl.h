#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "ModelRenderingComponent.h"
#include "TriangleMeshComponent.h"
#include "Types.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

class LitWithVertexColorRendererImpl
{
public:
  static LitWithVertexColorRendererImpl& getInstance();

  void Init();

  void Flush();

  void DrawTriangle(const glm::vec3& p1,
                    const glm::vec3& p1Color,
                    const glm::vec3& p1Normal,
                    const glm::vec3& p2,
                    const glm::vec3& p2Color,
                    const glm::vec3& p2Normal,
                    const glm::vec3& p3,
                    const glm::vec3& p3Color,
                    const glm::vec3& p3Normal);

  void DrawTriangles(const RawArrayView& vertexDataArray);
  TriangleMeshID AddStaticTriangles(const RawDataView& vertexData);

  void DeleteStaticTriangles(const TriangleMeshID& meshID);

  void DrawModelTransformed(const ModelID& modelID,
                            const glm::mat4x4& transform);

  ModelInstanceID AddStaticModel(const ModelID& modelID,
                                 const glm::mat4x4& transform);

  void DeleteStaticModel(const ModelInstanceID& instanceID);

private:
  void ConfigureShaderForTriangleDrawing();
  void ConfigureShaderForTransformedModelDrawing();

  LitWithVertexColorRendererImpl() = default;

  LitWithVertexColorRendererImpl(const LitWithVertexColorRendererImpl& other) =
    delete;
  LitWithVertexColorRendererImpl& operator=(
    const LitWithVertexColorRendererImpl& other) = delete;

  LitWithVertexColorRendererImpl(LitWithVertexColorRendererImpl&& other) =
    delete;
  LitWithVertexColorRendererImpl& operator=(
    LitWithVertexColorRendererImpl&& other) = delete;

  TriangleMeshComponent m_triangleMeshComponent;
  ModelRenderingComponent m_modelComponent;
};

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
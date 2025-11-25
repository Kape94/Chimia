#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "ModelRenderingComponent.h"
#include "ObjectTable.h"
#include "Renderers.h"
#include "TriangleMeshComponent.h"
#include "Types.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

class LitRendererImpl : public LitRenderer
{
public:
  static LitRendererImpl& getInstance();

  void Init();

  void Flush();

  void DrawTriangle(const glm::vec3& p1,
                    const glm::vec3& normal1,
                    const glm::vec3& p2,
                    const glm::vec3& normal2,
                    const glm::vec3& p3,
                    const glm::vec3& normal3,
                    const MaterialID& materialID) override;

  void DrawTriangles(const std::vector<float>& vertexData,
                     const MaterialID& materialID) override;

  void DrawTriangles(const std::vector<float>& vertexData,
                     const std::vector<unsigned>& indexData,
                     const MaterialID& materialID) override;

  TriangleMeshID AddStaticTriangles(const std::vector<float>& vertexData,
                                    const MaterialID& materialID) override;

  TriangleMeshID AddStaticTriangles(const std::vector<float>& vertexData,
                                    const std::vector<unsigned>& indexData,
                                    const MaterialID& materialID) override;

  void DeleteStaticTriangles(const TriangleMeshID& meshID) override;

  void DrawModelTransformed(const ModelID& modelID,
                            const glm::mat4x4& transform,
                            const MaterialID& materialID) override;

  ModelInstanceID AddStaticModel(const ModelID& modelID,
                                 const glm::mat4x4& transform,
                                 const MaterialID& materialID) override;

  void DeleteStaticModel(const ModelInstanceID& instanceID) override;

private:
  TriangleMeshComponent* FetchTriangleRenderComponentForMaterial(
    const MaterialID& materialID);

  void ConfigureShaderForTriangleDrawing(const MaterialID& materialID);
  void ConfigureShaderForTransformedModelDrawing();

  LitRendererImpl() = default;

  LitRendererImpl(const LitRendererImpl& other) = delete;
  LitRendererImpl& operator=(const LitRendererImpl& other) = delete;

  LitRendererImpl(LitRendererImpl&& other) = delete;
  LitRendererImpl& operator=(LitRendererImpl&& other) = delete;

  ObjectTable<TriangleMeshComponent> m_triangleMeshComponents;
  ModelRenderingComponent m_modelComponent;
};

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
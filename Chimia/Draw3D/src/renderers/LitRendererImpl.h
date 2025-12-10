#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Draw3DNamespaceDefs.h"
#include "ModelRenderingComponent.h"
#include "ObjectTable.h"
#include "TriangleMeshComponent.h"
#include "Types.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

class LitRendererImpl
{
public:
  static LitRendererImpl& getInstance();

  void Init();

  void Flush();

  void DrawTriangle(const glm::vec3& p1,
                    const glm::vec3& p1Normal,
                    const glm::vec3& p2,
                    const glm::vec3& p2Normal,
                    const glm::vec3& p3,
                    const glm::vec3& p3Normal,
                    const MaterialID& materialID);

  void DrawTriangles(const RawArrayView& vertexDataArray,
                     const MaterialID& materialID);
  TriangleMeshID AddStaticTriangles(const RawDataView& vertexData,
                                    const MaterialID& materialID);

  void DeleteStaticTriangles(const TriangleMeshID& meshID);

  void DrawModelTransformed(const ModelID& modelID,
                            const glm::mat4x4& transform,
                            const MaterialID& materialID);

  ModelInstanceID AddStaticModel(const ModelID& modelID,
                                 const glm::mat4x4& transform,
                                 const MaterialID& materialID);

  void DeleteStaticModel(const ModelInstanceID& instanceID);

private:
  TriangleMeshComponent* FetchTriangleRenderComponentForMaterial(
    const MaterialID& materialID);

  void ConfigureShaderForTriangleDrawing(const MaterialID& materialID);
  void ConfigureShaderForTransformedModelDrawing();

  DEFAULT_CONSTUCTIBLE(LitRendererImpl)
  NON_COPYABLE_NON_MOVABLE(LitRendererImpl)

  ObjectTable<TriangleMeshComponent> m_triangleMeshComponents;
  ModelRenderingComponent m_modelComponent;
};

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Draw3DNamespaceDefs.h"
#include "GenericRenderer.h"
#include "Types.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

class VertexColoredRendererImpl
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
                    const glm::vec3& color3);

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

  VertexColoredRendererImpl(
    const unsigned id,
    const Rendering::ShaderAttributes& vertexAttributes,
    const Rendering::ShaderAttributes& instancedAttributes,
    void (*setupShaderForTriangleRendering)(const ResourcesGroup&),
    void (*setupShaderForInstancedRendering)(const ResourcesGroup&));

  NON_DEFAULT_CONSTRUCTIBLE(VertexColoredRendererImpl)
  NON_COPYABLE_NON_MOVABLE(VertexColoredRendererImpl)

  GenericRenderer m_renderer;
};

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
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

class TexturedRendererImpl
{
public:
  static TexturedRendererImpl& getInstance();

  void Init();

  void Flush();

  void DrawTriangle(const glm::vec3& p1,
                    const glm::vec2& p1TexCoord,
                    const glm::vec3& p2,
                    const glm::vec2& p2TexCoord,
                    const glm::vec3& p3,
                    const glm::vec2& p3TexCoord,
                    const TextureID& textureID);

  void DrawTriangles(const RawArrayView& vertexDataArray,
                     const TextureID& textureID);
  TriangleMeshID AddStaticTriangles(const RawDataView& vertexData,
                                    const TextureID& textureID);

  void DeleteStaticTriangles(const TriangleMeshID& meshID);

  void DrawModelTransformed(const ModelID& modelID,
                            const glm::mat4x4& transform,
                            const TextureID& textureID);

  ModelInstanceID AddStaticModel(const ModelID& modelID,
                                 const glm::mat4x4& transform,
                                 const TextureID& textureID);

  void DeleteStaticModel(const ModelInstanceID& instanceID);

private:
  TexturedRendererImpl(
    const unsigned id,
    const Rendering::ShaderAttributes& vertexAttributes,
    const Rendering::ShaderAttributes& instancedAttributes,
    void (*setupShaderForTriangleRendering)(const ResourcesGroup&),
    void (*setupShaderForInstancedRendering)(const ResourcesGroup&));

  NON_DEFAULT_CONSTRUCTIBLE(TexturedRendererImpl)
  NON_COPYABLE_NON_MOVABLE(TexturedRendererImpl)

  GenericRenderer m_renderer;
};

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
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

class ColoredTexturedRendererImpl
{
public:
  static ColoredTexturedRendererImpl& getInstance();

  void Init();

  void Flush();

  void DrawTriangle(const glm::vec3& p1,
                    const glm::vec3& p1Color,
                    const glm::vec2& p1TexCoord,
                    const glm::vec3& p2,
                    const glm::vec3& p2Color,
                    const glm::vec2& p2TexCoord,
                    const glm::vec3& p3,
                    const glm::vec3& p3Color,
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
  TriangleMeshComponent* FetchTriangleRenderComponentForTexture(
    const TextureID& textureID);
  ModelRenderingComponent* FetchModelRenderComponentForTexture(
    const TextureID& textureID);

  void ConfigureShaderForTriangleDrawing(const TextureID& textureID);
  void ConfigureShaderForTransformedModelDrawing(const TextureID& textureID);

  DEFAULT_CONSTUCTIBLE(ColoredTexturedRendererImpl)
  NON_COPYABLE_NON_MOVABLE(ColoredTexturedRendererImpl)

  ObjectTable<TriangleMeshComponent> m_triangleMeshComponents;
  ObjectTable<ModelRenderingComponent> m_modelComponents;
};

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
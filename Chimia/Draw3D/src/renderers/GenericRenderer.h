#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Core/Types.h"
#include "Draw3DNamespaceDefs.h"
#include "ModelRenderingComponent.h"
#include "ObjectTable.h"
#include "Rendering/ShaderAttribute.h"
#include "ResourceGroup.h"
#include "TriangleMeshComponent.h"
#include "Types.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <initializer_list>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

class GenericRenderer
{
public:
  GenericRenderer(
    const unsigned id,
    const Rendering::ShaderAttributes& vertexAttributes,
    const Rendering::ShaderAttributes& instancedAttributes,
    void (*setupShaderForTriangleRendering)(const ResourcesGroup&),
    void (*setupShaderForInstancedRendering)(const ResourcesGroup&));

  void Init();

  void Flush();

  void DrawTriangle(const std::initializer_list<RawDataView>& vertexData,
                    const ResourceGroupID& resourcesID);

  void DrawTriangles(const RawArrayView& vertexDataArray,
                     const ResourceGroupID& resourcesID);
  TriangleMeshID AddStaticTriangles(const RawDataView& vertexData,
                                    const ResourceGroupID& textureID);

  void DeleteStaticTriangles(const TriangleMeshID& meshID);

  void DrawModelTransformed(const ModelID& modelID,
                            const glm::mat4x4& transform,
                            const ResourceGroupID& textureID);

  ModelInstanceID AddStaticModel(const ModelID& modelID,
                                 const glm::mat4x4& transform,
                                 const ResourceGroupID& textureID);

  void DeleteStaticModel(const ModelInstanceID& instanceID);

private:
  TriangleMeshComponent* FetchTriangleRenderComponentForTexture(
    const ResourceGroupID& textureID);
  ModelRenderingComponent* FetchModelRenderComponentForTexture(
    const ResourceGroupID& textureID);

  void ConfigureShaderForTriangleDrawing(const ResourceGroupID& textureID);
  void ConfigureShaderForTransformedModelDrawing(
    const ResourceGroupID& textureID);

  NON_DEFAULT_CONSTRUCTIBLE(GenericRenderer)
  NON_COPYABLE_NON_MOVABLE(GenericRenderer)

  const unsigned m_id;
  const Rendering::ShaderAttributes m_vertexAttributes;
  const Rendering::ShaderAttributes m_instancedAttributes;

  void (*const m_setupShaderForTriangleRendering)(const ResourcesGroup&) =
    nullptr;
  void (*const m_setupShaderForInstancedRendering)(const ResourcesGroup&) =
    nullptr;

  ObjectTable<TriangleMeshComponent> m_triangleMeshComponents;
  ObjectTable<ModelRenderingComponent> m_modelComponents;
};

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
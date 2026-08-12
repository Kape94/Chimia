#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Core/Types.h"
#include "Draw3DNamespaceDefs.h"
#include "ModelRenderingComponent.h"
#include "ObjectTable.h"
#include "Rendering/DataLayout.h"
#include "ResourceGroup.h"
#include "ShaderBindingsTemplate.h"
#include "TransitionRenderingComponent.h"
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
  DEFAULT_CONSTUCTIBLE(GenericRenderer)

  void Create(
    const unsigned id,
    const Rendering::DataLayout& vertexDataLayout,
    const Rendering::DataLayout& instancedDataLayout,
    const Rendering::DataLayout& transitionInstancedDataLayout,
    const ShaderBindingsTemplate& vertexBindingsTemplates,
    const ShaderBindingsTemplate& targetVertexBindingsTemplates,
    const ShaderBindingsTemplate& instancedBindingsTemplates,
    const ShaderBindingsTemplate& transitionInstancedBindingsTemplates,
    void (*setupShaderForTriangleRendering)(const ResourcesGroup&),
    void (*setupShaderForInstancedRendering)(const ResourcesGroup&),
    void (*setupShaderForTransitionRendering)(const ResourcesGroup&));

  void Init();

  void Flush(const eImmediateFlusingPolicy flushingPolicy);

  void DrawTriangle(const std::initializer_list<RawDataView>& vertexData,
                    const ResourceGroupID& resourcesID);

  void DrawTriangles(const RawArrayView& vertexDataArray,
                     const ResourceGroupID& resourcesID);
  TriangleMeshID AddRetainedTriangles(const RawDataView& vertexData,
                                      const ResourceGroupID& textureID);

  void DeleteRetainedTriangles(const TriangleMeshID& meshID);

  void DrawModelTransformed(const ModelID& modelID,
                            const glm::mat4x4& transform,
                            const ResourceGroupID& textureID);

  ModelInstanceID AddRetainedModel(const ModelID& modelID,
                                   const glm::mat4x4& transform,
                                   const ResourceGroupID& textureID);

  void DeleteRetainedModel(const ModelInstanceID& instanceID);

  void DrawTransition(const TransitionID& transition,
                      const float interpolation,
                      const glm::mat4x4& transform,
                      const ResourceGroupID& resource);

private:
  TriangleMeshComponent* FetchTriangleRenderComponentForResource(
    const ResourceGroupID& textureID);
  ModelRenderingComponent* FetchModelRenderComponentForResource(
    const ResourceGroupID& textureID);
  TransitionRenderingComponent* FetchTransitionRenderComponentForResource(
    const ResourceGroupID& textureID);

  void ConfigureShaderForTriangleDrawing(const ResourceGroupID& textureID);
  void ConfigureShaderForTransformedModelDrawing(
    const ResourceGroupID& textureID);
  void ConfigureShaderForTransitionDrawing(const ResourceGroupID& textureID);

  NON_COPYABLE_NON_MOVABLE(GenericRenderer)

  unsigned m_id;
  Rendering::DataLayout m_vertexDataLayout;
  Rendering::DataLayout m_instancedDataLayout;
  Rendering::DataLayout m_transitionInstancedDataLayout;
  ShaderBindingsTemplate m_vertexBindingsTemplates;
  ShaderBindingsTemplate m_instancedBindingsTemplates;
  ShaderBindingsTemplate m_targetVertexBindingsTemplates;
  ShaderBindingsTemplate m_transitionInstancedBindingsTemplates;

  void (*m_setupShaderForTriangleRendering)(const ResourcesGroup&) = nullptr;
  void (*m_setupShaderForInstancedRendering)(const ResourcesGroup&) = nullptr;
  void (*m_setupShaderForTransitionRendering)(const ResourcesGroup&) = nullptr;

  ObjectTable<TriangleMeshComponent> m_triangleMeshComponents;
  ObjectTable<ModelRenderingComponent> m_modelComponents;
  ObjectTable<TransitionRenderingComponent> m_transitionComponents;
};

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
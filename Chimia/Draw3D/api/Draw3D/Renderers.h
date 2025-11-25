#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "Types.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <vector>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
// Renderers interfaces
// ----------------------------------------------------------------------------

class VertexColoredRenderer
{
public:
  virtual void DrawTriangle(const glm::vec3& p1,
                            const glm::vec3& color1,
                            const glm::vec3& p2,
                            const glm::vec3& color2,
                            const glm::vec3& p3,
                            const glm::vec3& color3) = 0;

  virtual void DrawTriangles(const std::vector<float>& vertexData) = 0;

  virtual void DrawTriangles(const std::vector<float>& vertexData,
                             const std::vector<unsigned>& indexData) = 0;

  virtual TriangleMeshID AddStaticTriangles(
    const std::vector<float>& vertexData) = 0;

  virtual TriangleMeshID AddStaticTriangles(
    const std::vector<float>& vertexData,
    const std::vector<unsigned>& indexData) = 0;

  virtual void DeleteStaticTriangles(const TriangleMeshID& meshID) = 0;

  virtual void DrawModelTransformed(const ModelID& modelID,
                                    const glm::mat4x4& transform) = 0;

  virtual ModelInstanceID AddStaticModel(const ModelID& modelID,
                                         const glm::mat4x4& transform) = 0;

  virtual void DeleteStaticModel(const ModelInstanceID& instanceID) = 0;
};

class LitRenderer
{
public:
  virtual void DrawTriangle(const glm::vec3& p1,
                            const glm::vec3& normal1,
                            const glm::vec3& p2,
                            const glm::vec3& normal2,
                            const glm::vec3& p3,
                            const glm::vec3& normal3,
                            const MaterialID& materialID) = 0;

  virtual void DrawTriangles(const std::vector<float>& vertexData,
                             const MaterialID& materialID) = 0;

  virtual void DrawTriangles(const std::vector<float>& vertexData,
                             const std::vector<unsigned>& indexData,
                             const MaterialID& materialID) = 0;

  virtual TriangleMeshID AddStaticTriangles(
    const std::vector<float>& vertexData,
    const MaterialID& materialID) = 0;

  virtual TriangleMeshID AddStaticTriangles(
    const std::vector<float>& vertexData,
    const std::vector<unsigned>& indexData,
    const MaterialID& materialID) = 0;

  virtual void DeleteStaticTriangles(const TriangleMeshID& meshID) = 0;

  virtual void DrawModelTransformed(const ModelID& modelID,
                                    const glm::mat4x4& transform,
                                    const MaterialID& materialID) = 0;

  virtual ModelInstanceID AddStaticModel(const ModelID& modelID,
                                         const glm::mat4x4& transform,
                                         const MaterialID& materialID) = 0;

  virtual void DeleteStaticModel(const ModelInstanceID& instanceID) = 0;
};

// ----------------------------------------------------------------------------
// Renderer retrievers
// ----------------------------------------------------------------------------

VertexColoredRenderer&
GetVertexColoredRenderer();

LitRenderer&
GetLitRenderer();

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
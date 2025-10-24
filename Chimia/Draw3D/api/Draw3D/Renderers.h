#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

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

  virtual unsigned AddStaticTriangles(const std::vector<float>& vertexData) = 0;

  virtual void DeleteStaticTriangles(unsigned id) = 0;

  virtual unsigned CreateModel(const std::vector<float>& vertexData,
                               const std::vector<unsigned>& indices) = 0;

  virtual void DrawModelTransformed(unsigned modelID,
                                    const glm::mat4x4& transform) = 0;

  virtual void DrawIndexedTriangles(const std::vector<float>& vertexData,
                                    const std::vector<unsigned>& indexData) = 0;
};

// ----------------------------------------------------------------------------
// Renderer retrievers
// ----------------------------------------------------------------------------

VertexColoredRenderer&
GetVertexColoredRenderer();

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
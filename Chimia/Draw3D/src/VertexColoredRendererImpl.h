#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "Renderers.h"

#include "Bits/Buffer/RawBuffer.h"
#include "Rendering/Buffer.h"

#include <glm/vec3.hpp>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

class VertexColoredRendererImpl : public VertexColoredRenderer
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
                    const glm::vec3& color3) override;

private:
  VertexColoredRendererImpl() = default;

  VertexColoredRendererImpl(const VertexColoredRendererImpl& other) = delete;
  VertexColoredRendererImpl& operator=(const VertexColoredRendererImpl& other) =
    delete;

  VertexColoredRendererImpl(VertexColoredRendererImpl&& other) = delete;
  VertexColoredRendererImpl& operator=(VertexColoredRendererImpl&& other) =
    delete;

  Bits::RawBuffer m_triangleInputBuffer;
  Rendering::Buffer m_triangleGpuBuffer;
};

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
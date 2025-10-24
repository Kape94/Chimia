#include "StaticTriangles.h"
#include "Draw3DNamespaceDefs.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

void
StaticTriangles::Create(const std::vector<float>& vertexData,
                        const Rendering::ShaderAttributes& shaderAttributes)
{
  m_gpuBuffer.Create(vertexData, shaderAttributes);
}

// ----------------------------------------------------------------------------

void
StaticTriangles::Render() const
{
  m_gpuBuffer.Render();
}

// ----------------------------------------------------------------------------
#include "VertexColoredRendererImpl.h"

#include "CameraPrivate.h"
#include "Shaders.h"

#include "Rendering/Shader.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

VertexColoredRendererImpl&
VertexColoredRendererImpl::getInstance()
{
  static VertexColoredRendererImpl renderer;
  return renderer;
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::Init()
{
  const size_t triangleBatchSize = 100;
  const size_t vertexSizePerTriangle = 6 * sizeof(glm::vec3);
  const size_t bufferTotalSize = triangleBatchSize * vertexSizePerTriangle;

  m_triangleInputBuffer.Resize(bufferTotalSize);

  const float* vertexData = nullptr;
  const size_t nFloats = bufferTotalSize / sizeof(float);
  m_triangleGpuBuffer.Create(
    vertexData,
    nFloats,
    { Rendering::ShaderAttribute::Float(0 /*positionLoc*/, 3 /*nFloats*/),
      Rendering::ShaderAttribute::Float(1 /*colorLoc*/, 3 /*nFloats*/) });
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::Flush()
{
  const size_t inputSize = m_triangleInputBuffer.GetSize();
  if (inputSize == 0) {
    return;
  }

  const float* inputData =
    reinterpret_cast<float*>(m_triangleInputBuffer.GetData());
  const unsigned nFloatItems = inputSize / sizeof(float);

  m_triangleGpuBuffer.Load(inputData, nFloatItems);

  Rendering::Shader& shader = Shaders::VertexColored();
  shader.Use();
  shader.SetUniform("transform", CameraPrivate::GetCurrentTransform());

  m_triangleGpuBuffer.Render();

  m_triangleInputBuffer.Reset();
}

// ----------------------------------------------------------------------------

void
VertexColoredRendererImpl::DrawTriangle(const glm::vec3& p1,
                                        const glm::vec3& color1,
                                        const glm::vec3& p2,
                                        const glm::vec3& color2,
                                        const glm::vec3& p3,
                                        const glm::vec3& color3)
{
  constexpr size_t VEC3_SIZE = sizeof(glm::vec3);
  constexpr size_t INCOMING_SIZE = 6 * VEC3_SIZE;

  if (m_triangleInputBuffer.GetAvailableSize() < INCOMING_SIZE) {
    Flush();
  }

  m_triangleInputBuffer.Append(&p1, VEC3_SIZE);
  m_triangleInputBuffer.Append(&color1, VEC3_SIZE);
  m_triangleInputBuffer.Append(&p2, VEC3_SIZE);
  m_triangleInputBuffer.Append(&color2, VEC3_SIZE);
  m_triangleInputBuffer.Append(&p3, VEC3_SIZE);
  m_triangleInputBuffer.Append(&color3, VEC3_SIZE);
}

// ----------------------------------------------------------------------------
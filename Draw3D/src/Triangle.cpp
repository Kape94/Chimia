#include "Triangle.h"
#include "TrianglePrivate.h"

// ----------------------------------------------------------------------------

#include "CameraPrivate.h"
#include "Draw3DNamespaceDefs.h"
#include "Shaders.h"

#include "Bits/Buffer/RawBuffer.h"
#include "Rendering/Buffer.h"
#include "Rendering/Shader.h"
#include "Rendering/ShaderAttribute.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
// TriangleData
// ----------------------------------------------------------------------------

namespace TriangleData {

Chimia::Rendering::Buffer trianglesBuffer;

// TODO: batch size should be configurable
constexpr size_t BATCH_SIZE = 1000;
constexpr size_t TRIANGLE_SIZE = sizeof(glm::vec3) * 6;
constexpr size_t TOTAL_BUFFER_SIZE = TRIANGLE_SIZE * BATCH_SIZE;

Chimia::Bits::RawBuffer trianglesPos(TOTAL_BUFFER_SIZE);

}

// ----------------------------------------------------------------------------
// Triangle public
// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangle(const glm::vec3& p1,
                         const glm::vec3& p2,
                         const glm::vec3& p3,
                         const glm::vec3& color)
{
  Triangle(p1, color, p2, color, p3, color);
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::Triangle(const glm::vec3& p1,
                         const glm::vec3& p1Color,
                         const glm::vec3& p2,
                         const glm::vec3& p2Color,
                         const glm::vec3& p3,
                         const glm::vec3& p3Color)
{
  TrianglePrivate::Draw(p1, p1Color, p2, p2Color, p3, p3Color);
}

// ----------------------------------------------------------------------------
// Triangle private
// ----------------------------------------------------------------------------

void
Chimia::Draw3D::TrianglePrivate::Draw(const glm::vec3& p1,
                                      const glm::vec3& p1Color,
                                      const glm::vec3& p2,
                                      const glm::vec3& p2Color,
                                      const glm::vec3& p3,
                                      const glm::vec3& p3Color)
{
  using namespace TriangleData;

  constexpr size_t INCOMING_SIZE = TRIANGLE_SIZE;
  if (trianglesPos.GetAvailableSize() < INCOMING_SIZE) {
    TrianglePrivate::Flush();
  }

  trianglesPos.Append(&p1, sizeof(glm::vec3));
  trianglesPos.Append(&p1Color, sizeof(glm::vec3));
  trianglesPos.Append(&p2, sizeof(glm::vec3));
  trianglesPos.Append(&p2Color, sizeof(glm::vec3));
  trianglesPos.Append(&p3, sizeof(glm::vec3));
  trianglesPos.Append(&p3Color, sizeof(glm::vec3));
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::TrianglePrivate::Init()
{
  constexpr size_t N_FLOATS_PER_TRIANGLE = 18;
  constexpr size_t N_FLOATS_TOTAL =
    N_FLOATS_PER_TRIANGLE * TriangleData::BATCH_SIZE;

  const float* vertexData = nullptr;
  TriangleData::trianglesBuffer.Create(
    vertexData,
    N_FLOATS_TOTAL,
    { Chimia::Rendering::ShaderAttribute::Float(0, 3),
      Chimia::Rendering::ShaderAttribute::Float(1, 3) });
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::TrianglePrivate::Flush()
{
  using namespace TriangleData;

  const unsigned size = trianglesPos.GetSize() / sizeof(float);
  if (size == 0)
    return;

  auto f = reinterpret_cast<const float*>(trianglesPos.GetData());

  trianglesBuffer.Load(f, size);

  Rendering::Shader& shader = Draw3D::Shaders::VertexColored();
  shader.Use();
  shader.SetUniform("transform", CameraPrivate::GetCurrentTransform());

  trianglesBuffer.Render();

  trianglesPos.Reset();
}

// ----------------------------------------------------------------------------
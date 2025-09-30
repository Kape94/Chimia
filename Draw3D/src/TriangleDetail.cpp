#include "TriangleDetail.h"

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

namespace {

Chimia::Rendering::Buffer trianglesBuffer;

constexpr size_t BATCH_SIZE = 1000;
constexpr size_t TRIANGLE_SIZE = sizeof(glm::vec3) * 6;
constexpr size_t TOTAL_BUFFER_SIZE = TRIANGLE_SIZE * BATCH_SIZE;

Chimia::Bits::RawBuffer trianglesPos(TOTAL_BUFFER_SIZE);

constexpr size_t N_FLOATS_PER_TRIANGLE = 18;
constexpr size_t N_FLOATS_TOTAL = N_FLOATS_PER_TRIANGLE * BATCH_SIZE;
const float vertexItems[N_FLOATS_TOTAL] = { 0.0f };

}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::TriangleDetail::Draw(const glm::vec3& p1,
                                     const glm::vec3& p1Color,
                                     const glm::vec3& p2,
                                     const glm::vec3& p2Color,
                                     const glm::vec3& p3,
                                     const glm::vec3& p3Color)
{
  constexpr size_t INCOMING_SIZE = TRIANGLE_SIZE;
  if (trianglesPos.GetAvailableSize() < INCOMING_SIZE) {
    TriangleDetail::Flush();
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
Chimia::Draw3D::TriangleDetail::Init()
{
  trianglesBuffer.Create(vertexItems,
                         N_FLOATS_TOTAL,
                         { Chimia::Rendering::ShaderAttribute::Float(0, 3),
                           Chimia::Rendering::ShaderAttribute::Float(1, 3) });
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::TriangleDetail::Flush()
{
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
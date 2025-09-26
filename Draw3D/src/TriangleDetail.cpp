#include "TriangleDetail.h"

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "ShaderCodes.h"

#include "Bits/Buffer/RawBuffer.h"
#include "Rendering/Buffer.h"
#include "Rendering/Shader.h"
#include "Rendering/ShaderAttribute.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace {

Chimia::Rendering::Buffer trianglesBuffer;
Chimia::Rendering::Shader shader;

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
                                     const glm::vec3& p2,
                                     const glm::vec3& p3,
                                     const glm::vec3& color)
{
  constexpr size_t INCOMING_SIZE = TRIANGLE_SIZE;
  if (trianglesPos.GetAvailableSize() < INCOMING_SIZE) {
    TriangleDetail::Flush();
  }

  trianglesPos.Append(&p1, sizeof(glm::vec3));
  trianglesPos.Append(&color, sizeof(glm::vec3));
  trianglesPos.Append(&p2, sizeof(glm::vec3));
  trianglesPos.Append(&color, sizeof(glm::vec3));
  trianglesPos.Append(&p3, sizeof(glm::vec3));
  trianglesPos.Append(&color, sizeof(glm::vec3));
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::TriangleDetail::Init()
{
  shader.Create(ShaderCodes::Vertex::vertexColored,
                ShaderCodes::Fragment::colored);
  trianglesBuffer.Create(vertexItems,
                         N_FLOATS_TOTAL,
                         { Chimia::Rendering::ShaderAttribute::Float(0, 3),
                           Chimia::Rendering::ShaderAttribute::Float(1, 3) });
}

// ----------------------------------------------------------------------------

void
Chimia::Draw3D::TriangleDetail::Flush()
{
  auto f = reinterpret_cast<const float*>(trianglesPos.GetData());
  const unsigned size = trianglesPos.GetSize() / sizeof(float);

  trianglesBuffer.Load(f, size);

  shader.Use();
  trianglesBuffer.Render();

  trianglesPos.Reset();
}

// ----------------------------------------------------------------------------
#include "LitTriangle.h"
#include "Core/Types.h"
#include "LitTrianglePrivate.h"

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"
#include "Illumination.h"
#include "IlluminationPrivate.h"
#include "Shaders.h"

#include "Bits/Buffer/RawBuffer.h"
#include "Rendering/Buffer.h"
#include "Rendering/Shader.h"
#include "Rendering/ShaderAttribute.h"

#include <map>

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
// LitTriangleInternal
// ----------------------------------------------------------------------------

namespace LitTriangleInternal {

// ----------------------------------------------------------------------------

namespace Constants {
// TODO: batch size should be configurable
constexpr size_t BATCH_SIZE = 1000;
constexpr size_t TRIANGLE_SIZE = sizeof(glm::vec3) * 6;
constexpr size_t TOTAL_BUFFER_SIZE = TRIANGLE_SIZE * BATCH_SIZE;

}

// ----------------------------------------------------------------------------

namespace State {

Chimia::Rendering::Buffer trianglesBuffer;

// TODO: define material comparison properly
auto compareMaterial = [](const Material& a, const Material& b) {
  return a.ambient.x < b.ambient.x;
};
std::map<Material, Chimia::Bits::RawBuffer, decltype(compareMaterial)>
  dataPerMaterial(compareMaterial);

}

// ----------------------------------------------------------------------------

Chimia::Bits::RawBuffer&
RetrieveBufferForMaterial(const Material& material)
{
  // TODO: What to do if insertion fails?
  static Chimia::Bits::RawBuffer empty;

  auto it = State::dataPerMaterial.find(material);
  if (it != State::dataPerMaterial.end()) {
    return it->second;
  }

  auto insertionIt = State::dataPerMaterial.emplace(
    material, Chimia::Bits::RawBuffer(Constants::TOTAL_BUFFER_SIZE));
  const bool insertedWithSuccess = insertionIt.second;

  return insertedWithSuccess ? insertionIt.first->second : empty;
}

// ----------------------------------------------------------------------------

void
FlushBuffer(Chimia::Bits::RawBuffer& buffer, const Material& material)
{
  const unsigned frameInputSize = buffer.GetSize();
  if (frameInputSize == 0)
    return;

  State::trianglesBuffer.Load(
    Chimia::RawDataView{ buffer.GetData(), frameInputSize });

  Chimia::Rendering::Shader& shader = Chimia::Draw3D::Shaders::GouraudLit();
  shader.Use();
  IlluminationPrivate::ConfigureLightsOnShader(shader);
  IlluminationPrivate::ConfigureMaterialOnShader(material, shader);

  State::trianglesBuffer.Render();

  buffer.Reset();
}

}

// ----------------------------------------------------------------------------
// LitTriangle public
// ----------------------------------------------------------------------------

void
Chimia::Draw3D::LitTriangle(const glm::vec3& p1,
                            const glm::vec3& p1Normal,
                            const glm::vec3 p2,
                            const glm::vec3& p2Normal,
                            const glm::vec3 p3,
                            const glm::vec3& p3Normal,
                            const Material& material)
{
  Chimia::Bits::RawBuffer& buffer =
    LitTriangleInternal::RetrieveBufferForMaterial(material);

  constexpr size_t INCOMING_SIZE =
    LitTriangleInternal::Constants::TRIANGLE_SIZE;
  if (buffer.GetAvailableSize() < INCOMING_SIZE) {
    LitTriangleInternal::FlushBuffer(buffer, material);
  }

  buffer.Append(&p1, sizeof(glm::vec3));
  buffer.Append(&p1Normal, sizeof(glm::vec3));
  buffer.Append(&p2, sizeof(glm::vec3));
  buffer.Append(&p2Normal, sizeof(glm::vec3));
  buffer.Append(&p3, sizeof(glm::vec3));
  buffer.Append(&p3Normal, sizeof(glm::vec3));
}

// ----------------------------------------------------------------------------
// LitTriangle private
// ----------------------------------------------------------------------------

void
LitTrianglePrivate::Init()
{
  LitTriangleInternal::State::trianglesBuffer.Create(
    RawDataView{ nullptr, LitTriangleInternal::Constants::TOTAL_BUFFER_SIZE },
    { Chimia::Rendering::ShaderAttribute::Float(0, 3),
      Chimia::Rendering::ShaderAttribute::Float(1, 3) });
}

// ----------------------------------------------------------------------------

void
LitTrianglePrivate::Flush()
{
  for (auto& bufferWithMaterial : LitTriangleInternal::State::dataPerMaterial) {
    LitTriangleInternal::FlushBuffer(bufferWithMaterial.second,
                                     bufferWithMaterial.first);
  }
}

// ----------------------------------------------------------------------------
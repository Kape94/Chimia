#include "ModelsDrawingFixture.h"
#include "Core/Types.h"
#include "Draw3D/Resources.h"
#include "Draw3D/Types.h"

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <map>
#include <vector>

// ----------------------------------------------------------------------------

namespace SampleData {

// clang-format off
const std::vector<glm::vec3> positions{
    // first triangle
    {0.0f, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},

    // second triangle
    {0.0f, 0.0f, 0.0f},
    {-1.0f, 0.0f, 0.0f},
    {0.0f, -1.0f, 0.0f},
};

const std::vector<glm::vec4> colors{
    // first triangle
    {1.0f, 0.0f, 0.0f, 1.0f},
    {0.0f, 1.0f, 0.0f, 1.0f},
    {0.0f, 0.0f, 1.0f, 1.0f},

    // second triangle
    {1.0f, 0.0f, 0.0f, 1.0f},
    {0.0f, 1.0f, 0.0f, 1.0f},
    {0.0f, 0.0f, 1.0f, 1.0f},
};

constexpr float unit = 0.577350269189626f;
const std::vector<glm::vec3> normals{
    // first triangle
    {0.0f, 0.0f, -1.0f},
    {unit, -unit, -unit},
    {-unit, unit, -unit},

    // second triangle
    {0.0f, 0.0f, -1.0f},
    {-unit, unit, -unit},
    {unit, -unit, -unit},
};

const std::vector<glm::vec2> texCoords{
    // first triangle
    {0.0f, 0.0f},
    {1.0f, 0.0f},
    {0.0f, 1.0f},

    // second triangle
    {0.0f, 0.0f},
    {1.0f, 0.0f},
    {0.0f, 1.0f},
};

const std::vector<unsigned> indices {
    0, 1, 2,
    3, 4, 5
};
// clang-format on

}

// ----------------------------------------------------------------------------

namespace ModelsFixtureInternal {
bool g_isInitialized = false;

using ModelPtr = std::unique_ptr<Chimia::Draw3D::ModelID>;
std::map<Chimia::Draw3D::eVertexLayout, ModelPtr> g_referenceModels;

std::vector<glm::mat4x4> g_referenceTransforms;

std::vector<float>
ToVector(const glm::vec2& v)
{
  return { v.x, v.y };
}

std::vector<float>
ToVector(const glm::vec3& v)
{
  return { v.x, v.y, v.z };
}

std::vector<float>
ToVector(const glm::vec4& v)
{
  return { v.r, v.g, v.b, v.a };
}

std::vector<float>
VertexData(const Chimia::Draw3D::eVertexLayout layout)
{
  auto append = [](std::vector<float>& dest, const std::vector<float>& source) {
    dest.insert(dest.end(), source.begin(), source.end());
  };

  const size_t nVertices = SampleData::positions.size();

  std::vector<float> vertexData;
  for (size_t i = 0; i < nVertices; ++i) {
    append(vertexData, ToVector(SampleData::positions[i]));
    if (layout == Chimia::Draw3D::eVertexLayout::POSITION3_COLOR4 ||
        layout == Chimia::Draw3D::eVertexLayout::POSITION3_COLOR4_NORMAL3 ||
        layout == Chimia::Draw3D::eVertexLayout::POSITION3_COLOR4_TEXCOORD2 ||
        layout ==
          Chimia::Draw3D::eVertexLayout::POSITION3_COLOR4_NORMAL3_TEXCOORD2) {
      append(vertexData, ToVector(SampleData::colors[i]));
    }
    if (layout == Chimia::Draw3D::eVertexLayout::POSITION3_NORMAL3 ||
        layout == Chimia::Draw3D::eVertexLayout::POSITION3_COLOR4_NORMAL3 ||
        layout == Chimia::Draw3D::eVertexLayout::POSITION3_NORMAL3_TEXCOORD2 ||
        layout ==
          Chimia::Draw3D::eVertexLayout::POSITION3_COLOR4_NORMAL3_TEXCOORD2) {
      append(vertexData, ToVector(SampleData::normals[i]));
    }
    if (layout == Chimia::Draw3D::eVertexLayout::POSITION3_TEXCOORD2 ||
        layout == Chimia::Draw3D::eVertexLayout::POSITION3_COLOR4_TEXCOORD2 ||
        layout == Chimia::Draw3D::eVertexLayout::POSITION3_NORMAL3_TEXCOORD2 ||
        layout ==
          Chimia::Draw3D::eVertexLayout::POSITION3_COLOR4_NORMAL3_TEXCOORD2) {
      append(vertexData, ToVector(SampleData::texCoords[i]));
    }
  }

  return vertexData;
}

void
CreateTransforms()
{
  const glm::mat4x4 identity = glm::identity<glm::mat4x4>();

  const glm::mat4x4 t1 = glm::translate(identity, { -0.5f, 0.0f, 0.0f }) *
                         glm::scale(identity, { 0.2f, 0.2f, 0.2f });

  const glm::mat4x4 t2 = glm::translate(identity, { 0.5f, 0.0f, 0.0f }) *
                         glm::scale(identity, { 0.2f, 0.2f, 0.2f });

  constexpr float PI = 3.141592;
  const glm::mat4x4 t3 = glm::translate(identity, { 0.0f, 0.5f, 0.0f }) *
                         glm::scale(identity, { 0.2f, 0.2f, 0.2f }) *
                         glm::rotate(identity, PI / 4.0f, { 0.0f, 0.0f, 1.0f });

  g_referenceTransforms.push_back(t1);
  g_referenceTransforms.push_back(t2);
  g_referenceTransforms.push_back(t3);
}

void
AddReferenceModel(const Chimia::Draw3D::eVertexLayout& layout)
{
  const auto vertexData = VertexData(layout);
  Chimia::Draw3D::MeshDataView mesh(Chimia::RawDataView(vertexData),
                                    SampleData::positions.size(),
                                    Chimia::RawArrayView(SampleData::indices));

  g_referenceModels[layout].reset(
    new Chimia::Draw3D::ModelID(Chimia::Draw3D::CreateModel(mesh, layout)));
}

void
CreateReferenceModels()
{
  AddReferenceModel(Chimia::Draw3D::eVertexLayout::POSITION3_COLOR4);
  AddReferenceModel(Chimia::Draw3D::eVertexLayout::POSITION3_NORMAL3);
  AddReferenceModel(Chimia::Draw3D::eVertexLayout::POSITION3_TEXCOORD2);
  AddReferenceModel(Chimia::Draw3D::eVertexLayout::POSITION3_COLOR4_NORMAL3);
  AddReferenceModel(Chimia::Draw3D::eVertexLayout::POSITION3_COLOR4_TEXCOORD2);
  AddReferenceModel(
    Chimia::Draw3D::eVertexLayout::POSITION3_COLOR4_NORMAL3_TEXCOORD2);
}

void
Setup()
{
  if (g_isInitialized) {
    return;
  }

  CreateTransforms();
  CreateReferenceModels();

  g_isInitialized = true;
}
}

// ----------------------------------------------------------------------------

void
ModelsDrawingFixture::Init(const CommonTestingConfig& testingInfo)
{
  CommonTestingFixture::Init(testingInfo);

  ModelsFixtureInternal::Setup();
}

// ----------------------------------------------------------------------------

const Chimia::Draw3D::ModelID&
ModelsDrawingFixture::GetModel(const Chimia::Draw3D::eVertexLayout& layout)
{
  auto it = ModelsFixtureInternal::g_referenceModels.find(layout);
  assert(it != ModelsFixtureInternal::g_referenceModels.end() &&
         "ModelsFixture::GetModel: didn't find model for layout");

  return *it->second;
}

// ----------------------------------------------------------------------------

const std::vector<glm::mat4x4>&
ModelsDrawingFixture::GetTransforms()
{
  return ModelsFixtureInternal::g_referenceTransforms;
}

// ----------------------------------------------------------------------------
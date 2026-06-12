#include "GraphicsTestsData.h"

// ----------------------------------------------------------------------------

namespace GraphicsTestsDataInternals {
std::vector<glm::vec3> quad1Positions{
  { -0.5f, -0.5f, 0.0f },
  { 0.5f, -0.5f, 0.0f },
  { 0.5f, 0.5f, 0.0f },
  { -0.5f, 0.5f, 0.0f },

};

std::vector<glm::vec4> quad1Colors{ { 1.0f, 0.0f, 0.0f, 1.0f },
                                    { 0.0f, 1.0f, 0.0f, 1.0f },
                                    { 0.0f, 0.0f, 1.0f, 1.0f },
                                    { 1.0f, 0.0f, 1.0f, 1.0f } };

const float unitComponent = 0.577350269189626f;
std::vector<glm::vec3> quadNormals{
  { -unitComponent, -unitComponent, -unitComponent },
  { unitComponent, -unitComponent, -unitComponent },
  { unitComponent, unitComponent, -unitComponent },
  { -unitComponent, unitComponent, -unitComponent }
};

std::vector<glm::vec2> quadTexCoords{ { 0.0f, 0.0f },
                                      { 1.0f, 0.0f },
                                      { 1.0f, 1.0f },
                                      { 0.0f, 1.0f } };

std::vector<glm::vec3> quad2Positions{
  { -1.0f, -1.0f, 0.0f },
  { 0.0f, -1.0f, 0.0f },
  { 0.0f, 1.0f, 0.0f },
  { -1.0f, 1.0f, 0.0f },
};

std::vector<glm::vec4> quad2Colors{ { 1.0f, 0.0f, 0.0f, 1.0f },
                                    { 0.0f, 1.0f, 0.0f, 1.0f },
                                    { 0.0f, 0.0f, 1.0f, 1.0f },
                                    { 1.0f, 0.0f, 1.0f, 1.0f } };

std::vector<glm::vec3> quad3Positions{
  { 0.5f, -1.0f, 0.0f },
  { 1.0f, -1.0f, 0.0f },
  { 1.0f, 0.0f, 0.0f },
  { 0.5f, 0.0f, 0.0f },
};

std::vector<glm::vec4> quad3Colors{ { 1.0f, 0.0f, 1.0f, 1.0f },
                                    { 0.0f, 1.0f, 1.0f, 1.0f },
                                    { 0.0f, 0.0f, 1.0f, 1.0f },
                                    { 1.0f, 0.0f, 1.0f, 1.0f } };

std::vector<std::vector<glm::vec3>> quadPositions{ quad1Positions,
                                                   quad2Positions,
                                                   quad3Positions };

std::vector<std::vector<glm::vec4>> quadColors{ quad1Colors,
                                                quad2Colors,
                                                quad3Colors };
}

using namespace GraphicsTestsDataInternals;

std::vector<Chimia::Draw3D::VertexPCNT>
GraphicsTestsData::QuadPCNT(size_t quadNumber)
{
  const size_t nQuads = NQuads();
  auto vertex = [quadNumber, nQuads](size_t i) {
    return Chimia::Draw3D::VertexPCNT{ quadPositions[quadNumber % nQuads][i],
                                       quadColors[quadNumber % nQuads][i],
                                       quadNormals[i],
                                       quadTexCoords[i] };
  };
  return { vertex(0), vertex(1), vertex(2), vertex(3) };
}

std::vector<Chimia::Draw3D::VertexPC>
GraphicsTestsData::QuadPC(size_t quadNumber)
{
  auto quad = QuadPCNT(quadNumber);
  auto vertex = [quad](size_t i) {
    return Chimia::Draw3D::VertexPC{
      quad[i].position,
      quad[i].color,
    };
  };
  return { vertex(0), vertex(1), vertex(2), vertex(3) };
}

std::vector<Chimia::Draw3D::VertexPN>
GraphicsTestsData::QuadPN(size_t quadNumber)
{
  auto quad = QuadPCNT(quadNumber);
  auto vertex = [quad](size_t i) {
    return Chimia::Draw3D::VertexPN{
      quad[i].position,
      quad[i].normal,
    };
  };
  return { vertex(0), vertex(1), vertex(2), vertex(3) };
}

std::vector<Chimia::Draw3D::VertexPT>
GraphicsTestsData::QuadPT(size_t quadNumber)
{
  auto quad = QuadPCNT(quadNumber);
  auto vertex = [quad](size_t i) {
    return Chimia::Draw3D::VertexPT{
      quad[i].position,
      quad[i].texCoord,
    };
  };
  return { vertex(0), vertex(1), vertex(2), vertex(3) };
}

std::vector<Chimia::Draw3D::VertexPCN>
GraphicsTestsData::QuadPCN(size_t quadNumber)
{
  auto quad = QuadPCNT(quadNumber);
  auto vertex = [quad](size_t i) {
    return Chimia::Draw3D::VertexPCN{
      quad[i].position,
      quad[i].color,
      quad[i].normal,
    };
  };
  return { vertex(0), vertex(1), vertex(2), vertex(3) };
}

std::vector<Chimia::Draw3D::VertexPCT>
GraphicsTestsData::QuadPCT(size_t quadNumber)
{
  auto quad = QuadPCNT(quadNumber);
  auto vertex = [quad](size_t i) {
    return Chimia::Draw3D::VertexPCT{
      quad[i].position,
      quad[i].color,
      quad[i].texCoord,
    };
  };
  return { vertex(0), vertex(1), vertex(2), vertex(3) };
}

// ----------------------------------------------------------------------------

size_t
GraphicsTestsData::NQuads()
{
  return std::max(quadPositions.size(), quadColors.size());
}

// ----------------------------------------------------------------------------
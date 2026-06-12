#pragma once

#include <vector>

#include "Draw3D/Triangle.h"
#include "Draw3D/Types.h"

#include <functional>

namespace GraphicsTestsData {

std::vector<Chimia::Draw3D::VertexPCNT>
QuadPCNT(size_t quadNumber);

std::vector<Chimia::Draw3D::VertexPC>
QuadPC(size_t quadNumber);

std::vector<Chimia::Draw3D::VertexPN>
QuadPN(size_t quadNumber);

std::vector<Chimia::Draw3D::VertexPT>
QuadPT(size_t quadNumber);

std::vector<Chimia::Draw3D::VertexPCN>
QuadPCN(size_t quadNumber);

std::vector<Chimia::Draw3D::VertexPCT>
QuadPCT(size_t quadNumber);

size_t
NQuads();

template<typename QuadLayout>
void
DrawAllQuads(const std::function<std::vector<QuadLayout>(size_t)>& getQuad)
{
  const size_t nQuads = NQuads();
  for (size_t i = 0; i < nQuads; ++i) {
    auto quad = getQuad(i);

    Chimia::Draw3D::Triangle(quad[0], quad[1], quad[2]);
    Chimia::Draw3D::Triangle(quad[2], quad[3], quad[0]);
  }
}

template<typename QuadLayout, typename Resource>
void
DrawAllQuadsWithResource(
  const std::function<std::vector<QuadLayout>(size_t)>& getQuad,
  const Resource& resource)
{
  const size_t nQuads = NQuads();
  for (size_t i = 0; i < nQuads; ++i) {
    auto quad = getQuad(i);

    Chimia::Draw3D::Triangle(quad[0], quad[1], quad[2], resource);
    Chimia::Draw3D::Triangle(quad[2], quad[3], quad[0], resource);
  }
}
}
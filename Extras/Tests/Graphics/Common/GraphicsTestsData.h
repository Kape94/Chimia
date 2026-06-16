#pragma once

#include <vector>

#include "Draw3D/Draw3D.h"
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
DrawAllQuads(const std::function<std::vector<QuadLayout>(size_t)>& getQuad,
             unsigned flushOnEvery = 1000)
{
  const size_t nQuads = NQuads();
  for (size_t i = 0; i < nQuads; ++i) {
    auto quad = getQuad(i);

    Chimia::Draw3D::Triangle(quad[0], quad[1], quad[2]);
    Chimia::Draw3D::Triangle(quad[2], quad[3], quad[0]);

    if ((i + 1) % flushOnEvery == 0) {
      Chimia::Draw3D::Flush();
    }
  }
}

template<typename QuadLayout, typename Resource>
void
DrawAllQuadsWithResource(
  const std::function<std::vector<QuadLayout>(size_t)>& getQuad,
  const Resource& resource,
  unsigned flushOnEvery = 1000)
{
  const size_t nQuads = NQuads();
  for (size_t i = 0; i < nQuads; ++i) {
    auto quad = getQuad(i);

    Chimia::Draw3D::Triangle(quad[0], quad[1], quad[2], resource);
    Chimia::Draw3D::Triangle(quad[2], quad[3], quad[0], resource);

    if ((i + 1) % flushOnEvery == 0) {
      Chimia::Draw3D::Flush();
    }
  }
}
}
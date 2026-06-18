#pragma once

#include "Draw3D/Types.h"

#include <string>

// ----------------------------------------------------------------------------

struct TrianglesDrawingTestInfo
{
  std::string testName = "";
  unsigned flushOnEvery = 1000;
  bool shouldVerifyRetainedRemovals = true;
};

// ----------------------------------------------------------------------------

namespace QuadsDrawingFixture {
void
Init(const TrianglesDrawingTestInfo& testInfo);

std::string
FullArtifactName(const std::string& artifactName);

unsigned
FlushOnEvery();

bool
ShouldVerifyRetainedRemovals();

const Chimia::Draw3D::MaterialID&
ReferenceMaterial();

const Chimia::Draw3D::TextureID&
ReferenceTexture();

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
}

// ----------------------------------------------------------------------------
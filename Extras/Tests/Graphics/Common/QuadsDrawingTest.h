#pragma once

#include "Draw3D/Types.h"

#include <string>

// ----------------------------------------------------------------------------

struct ImmediateTrianglesTestInfo
{
  std::string testName = "";
  unsigned flushOnEvery = 1000;
};

// ----------------------------------------------------------------------------

namespace QuadsDrawingTest {
void
Init(const ImmediateTrianglesTestInfo& testInfo);

std::string
FullArtifactName(const std::string& artifactName);

unsigned
FlushOnEvery();

const Chimia::Draw3D::MaterialID&
ReferenceMaterial();

const Chimia::Draw3D::TextureID&
ReferenceTexture();
}

// ----------------------------------------------------------------------------
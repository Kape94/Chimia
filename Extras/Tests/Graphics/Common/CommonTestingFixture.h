#pragma once

#include "Draw3D/Types.h"

#include <string>

// ----------------------------------------------------------------------------

struct CommonTestingConfig
{
  std::string testName = "";
  unsigned flushOnEvery = 1000;
  bool shouldVerifyRetainedRemovals = true;
};

// ----------------------------------------------------------------------------

namespace CommonTestingFixture {
void
Init(const CommonTestingConfig& testInfo);

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
}

// ----------------------------------------------------------------------------
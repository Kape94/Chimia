#pragma once

// ----------------------------------------------------------------------------

#include "CommonTestingFixture.h"

#include "Draw3D/Types.h"
#include <glm/ext/matrix_float4x4.hpp>

// ----------------------------------------------------------------------------

namespace ModelsDrawingFixture {

void
Init(const CommonTestingConfig& testingInfo);

const Chimia::Draw3D::ModelID&
GetModel(const Chimia::Draw3D::eVertexLayout& layout);

const std::vector<glm::mat4x4>&
GetTransforms();
}

// ----------------------------------------------------------------------------
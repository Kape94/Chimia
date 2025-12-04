#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include <initializer_list>
#include <string>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace ShaderCodebase {
void
RegisterCodes(const std::initializer_list<std::pair<std::string, std::string>>&
                shaderCodeList);

void
RegisterPieceOfCode(const std::string& tag, const std::string& code);

std::string
Code(const std::string& tag);
}

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include <initializer_list>
#include <map>
#include <string>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace ShaderCodebase {
void
AddCodes(const std::initializer_list<std::pair<std::string, std::string>>&
           shaderCodeList);

void
AddToCodebase(const std::string& tag, const std::string& code);

std::string
Code(const std::string& tag);

std::string
Code(const std::string& tag,
     const std::map<std::string, std::string>& embeddedCodes);
}

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

#pragma once

#include "AppDefs.h"

BEGIN_CHIMIA_APP_NAMESPACE

namespace OpenGLHelper {

void
Init(const unsigned screenWidth, const unsigned screenHeight);

void
Clear(const float r, const float g, const float b);

void
Flush();

}

END_CHIMIA_APP_NAMESPACE

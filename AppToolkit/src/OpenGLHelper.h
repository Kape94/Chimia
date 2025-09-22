#pragma once

#include "AppToolkitDefs.h"

_BEGIN_APP_TOOLKIT_NAMESPACE

namespace OpenGLHelper {

void
Init(const unsigned screenWidth, const unsigned screenHeight);

void
Clear(const float r, const float g, const float b);

void
Flush();

}

_END_APP_TOOLKIT_NAMESPACE

#pragma once

#include "Utils/Window.h"

namespace TestsUtils {

void
InitTesting(char** argv, const Window& window);

void
ExpectImage(const std::string& imageFileName);
}
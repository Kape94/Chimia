#pragma once

#include "Utils/Window.h"

namespace TestsUtils {

void
InitTesting(char** argv, const Window& window);

std::string
GetTestingDirectory();

void
ExpectImage(const std::string& imageFileName);

namespace Development {
void
ActivateRGBImageLogging();

void
ActivateRGBAImageLogging();
}
}
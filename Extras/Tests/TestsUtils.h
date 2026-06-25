#pragma once

#include "Utils/Window.h"

namespace TestsUtils {

void
InitTesting(char** argv, const Window& window);

std::string
GetTestingDirectory();

void
ExpectImage(const std::string& imageFileName);

void
ExpectValue(const float value, const float expected, const float error);

namespace Development {
void
ActivateRGBImageLogging();

void
ActivateRGBAImageLogging();
}
}
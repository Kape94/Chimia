#pragma once

#include "Window.h"

#include <functional>
#include <string>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace SamplesUtils {
std::string
GetCurrentAppDir(char** argv);

void
InitRandom();
float
NormalizedRand();
int
Rand();

void
SyncForTargetFPS(unsigned fps);

void
BeginFrameStats();
void
LogFrameStats();

void
DoAfter(const std::function<void(void)>& action, const unsigned milliseconds);
void
DoAfterSync(const std::function<void(void)>& action,
            const unsigned milliseconds);
void
PollDeferredActions();

void
PollSingleDeferredAction();

void
SaveScreenshot(const Window& window, const std::string& imagePath);

}
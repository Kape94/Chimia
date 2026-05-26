#pragma once

#include "Window.h"

#include <functional>
#include <string>

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

struct Image
{
  unsigned char* data = nullptr;
  int width = 0;
  int height = 0;
  int nChannels = 0;
};

void
SaveScreenshot(const Window& window, const std::string& imagePath);

Image
ReadImage(const std::string& imagePath);

void
FreeImageData(Image& image);

}
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

glm::vec3
GetRGBPixel(const int i, const int j, const Image& image);

glm::vec4
GetRGBAPixel(const int i, const int j, const Image& image);

void
SetRGBPixel(const glm::vec3& color, const int i, const int j, Image& image);

void
SetRGBAPixel(const glm::vec4& color, const int i, const int j, Image& image);

float
ImageDifferece(const Image& i1, const Image& i2);

void
FreeImageData(Image& image);

}
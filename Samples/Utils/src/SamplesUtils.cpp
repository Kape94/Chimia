#include "SamplesUtils.h"

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <thread>

// ----------------------------------------------------------------------------

namespace {
std::chrono::time_point<std::chrono::high_resolution_clock>
CurrentTimePoint()
{
  return std::chrono::high_resolution_clock::now();
}

std::chrono::time_point<std::chrono::high_resolution_clock> g_lastCheck =
  CurrentTimePoint();
}

// ----------------------------------------------------------------------------

std::string
SamplesUtils::GetCurrentAppDir(char** argv)
{
  const std::filesystem::path p = argv[0];
  const std::filesystem::path current = std::filesystem::current_path();

  std::filesystem::path finalPath = current / p;
  finalPath.remove_filename();

  return finalPath.string();
}

// ----------------------------------------------------------------------------

void
SamplesUtils::InitRandom()
{
  srand(time(NULL));
}

// ----------------------------------------------------------------------------

float
SamplesUtils::NormalizedRand()
{
  return (float)rand() / RAND_MAX;
}

// ----------------------------------------------------------------------------

int
SamplesUtils::Rand()
{
  return rand();
}

// ----------------------------------------------------------------------------

void
SamplesUtils::SyncForTargetFPS(unsigned fps)
{
  const float frameTime = 1.0f / (float)fps;
  const long frameTimeInMilisecs = (long)(frameTime * 1000.0f);

  const auto current = CurrentTimePoint();
  const auto diff = current - g_lastCheck;
  
  const long durationInMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
  const long remainingFrameTime = frameTimeInMilisecs - durationInMilliseconds;

  std::this_thread::sleep_for(std::chrono::milliseconds(remainingFrameTime));
  g_lastCheck = CurrentTimePoint();
}

// ----------------------------------------------------------------------------
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
  const float frameTime = (float)1.0f / fps;

  const auto current = CurrentTimePoint();
  const auto diff = current - g_lastCheck;

  std::this_thread::sleep_for(diff);
  g_lastCheck = CurrentTimePoint();
}

// ----------------------------------------------------------------------------
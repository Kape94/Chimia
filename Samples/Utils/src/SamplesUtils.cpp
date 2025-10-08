#include "SamplesUtils.h"

#include <cstdlib>
#include <ctime>
#include <filesystem>

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
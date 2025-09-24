#include "SamplesUtils.h"

#include <filesystem>

// ----------------------------------------------------------------------------

std::string
SamplesUtils::GetCurrentAppDir(char** argv)
{
  const std::filesystem::path p = argv[0];
  const std::filesystem::path current = std::filesystem::current_path();

  std::filesystem::path final = current / p;
  final.remove_filename();

  return final;
}

// ----------------------------------------------------------------------------
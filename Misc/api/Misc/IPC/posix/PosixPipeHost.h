#pragma once

#include "Misc/Common/MiscNamespaceDefs.h"

#include <string>

BEGIN_MISC_NAMESPACE

class PosixPipeHost
{
public:
  PosixPipeHost(const std::string& _path);

  ~PosixPipeHost();

  void Create();
  void Close();

  std::string Read();

private:
  const std::string path;
  int fdRead = -1;
};

END_MISC_NAMESPACE

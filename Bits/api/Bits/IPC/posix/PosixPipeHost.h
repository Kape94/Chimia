#pragma once

#include "Bits/Common/BitsNamespaceDefs.h"

#include <string>

BEGIN_BITS_NAMESPACE

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

END_BITS_NAMESPACE

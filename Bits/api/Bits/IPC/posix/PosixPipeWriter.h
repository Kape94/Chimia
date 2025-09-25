#pragma once

#include "Bits/Common/BitsNamespaceDefs.h"

#include <string>

BEGIN_BITS_NAMESPACE

class PosixPipeWriter
{
public:
  PosixPipeWriter(const std::string& _path);

  ~PosixPipeWriter();

  void Write(const std::string& message);

private:
  const std::string path;
  int fdWrite = -1;
};

END_BITS_NAMESPACE

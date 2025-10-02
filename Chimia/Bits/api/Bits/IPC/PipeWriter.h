#pragma once

#include "Bits/Common/BitsNamespaceDefs.h"

#include "Bits/IPC/posix/PosixPipeWriter.h"

#include <string>

BEGIN_BITS_NAMESPACE

class PipeWriter
{
public:
  PipeWriter(const std::string& path);

  ~PipeWriter();

  void Write(const std::string& message);

private:
  Chimia::Bits::PosixPipeWriter pipe;
};

END_BITS_NAMESPACE

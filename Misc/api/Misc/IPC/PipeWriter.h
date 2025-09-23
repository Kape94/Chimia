#pragma once

#include "Misc/Common/MiscNamespaceDefs.h"

#include "Misc/IPC/posix/PosixPipeWriter.h"

#include <string>

BEGIN_MISC_NAMESPACE

class PipeWriter
{
public:
  PipeWriter(const std::string& path);

  ~PipeWriter();

  void Write(const std::string& message);

private:
  Misc::PosixPipeWriter pipe;
};

END_MISC_NAMESPACE

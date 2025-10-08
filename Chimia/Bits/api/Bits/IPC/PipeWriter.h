#pragma once

#include "Bits/Common/BitsNamespaceDefs.h"

#include <memory>
#include <string>

BEGIN_BITS_NAMESPACE

class PipeWriter
{
public:
  PipeWriter(const std::string& path);

  ~PipeWriter();

  void Write(const std::string& message);

private:
  class PlatformImpl;
  std::unique_ptr<PlatformImpl> m_impl;
};

END_BITS_NAMESPACE

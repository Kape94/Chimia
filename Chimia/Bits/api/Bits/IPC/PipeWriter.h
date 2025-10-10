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

  PipeWriter(const PipeWriter& other) = delete;
  PipeWriter& operator=(const PipeWriter& other) = delete;

  PipeWriter(PipeWriter&& other) = delete;
  PipeWriter& operator=(PipeWriter&& other) = delete;

  void Write(const std::string& message);

private:
  class PlatformImpl;
  std::unique_ptr<PlatformImpl> m_impl;
};

END_BITS_NAMESPACE

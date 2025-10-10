#pragma once

#include "Bits/Common/BitsNamespaceDefs.h"

#include <memory>
#include <string>

BEGIN_BITS_NAMESPACE

class ProcessInfo
{
public:
  ProcessInfo(const int pid);
  ~ProcessInfo();

  ProcessInfo(const ProcessInfo& other) = delete;
  ProcessInfo& operator=(const ProcessInfo& other) = delete;

  ProcessInfo(ProcessInfo&& other) = delete;
  ProcessInfo& operator=(ProcessInfo&& other) = delete;

  bool IsRunning() const;
  std::string GetName() const;
  double GetTimestamp() const;

private:
  class PlatformImpl;
  std::unique_ptr<PlatformImpl> m_impl;
};

END_BITS_NAMESPACE

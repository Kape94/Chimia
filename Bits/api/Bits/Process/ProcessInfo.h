#pragma once

#include "Bits/Common/BitsNamespaceDefs.h"

#include <string>

BEGIN_BITS_NAMESPACE

class ProcessInfo
{
public:
  ProcessInfo(const int pid);

  bool IsRunning() const;
  std::string GetName() const;
  double GetTimestamp() const;

private:
  double ReadTimestampFromStat() const;
  bool IsValid() const;
  void Invalidate();

  std::string ProcessDirectory() const;

  double m_processTimestamp = 0.0;
  int m_pid = 0;
};

END_BITS_NAMESPACE

#pragma once

#include "Bits/Common/BitsNamespaceDefs.h"

#include <functional>
#include <string>
#include <thread>
#include <vector>

BEGIN_BITS_NAMESPACE

class ChildProcess
{
public:
  ChildProcess(const std::string& processPath,
               const std::vector<std::string>& args);

  ~ChildProcess();

  ChildProcess(const ChildProcess& other) = delete;
  ChildProcess& operator=(const ChildProcess& other) = delete;

  ChildProcess(ChildProcess&& other) = delete;
  ChildProcess& operator=(ChildProcess&& other) = delete;

  void Await();

  void Terminate();

  int GetPID() const;

  using ProcessExitFn = std::function<void(int)>;
  void OnExit(ProcessExitFn fn);

  void OnTerminated(ProcessExitFn fn);

private:
  void ExecuteChildProcess(const std::string& processPath,
                           const std::vector<std::string>& args);

  void WatchProcess();

  ProcessExitFn m_onExit = nullptr;
  ProcessExitFn m_onTerminate = nullptr;

  std::thread m_processWatcher;

  class PlatformImpl;
  std::unique_ptr<PlatformImpl> m_impl;
};

END_BITS_NAMESPACE
#pragma once

#include "Misc/Common/MiscNamespaceDefs.h"

#include <functional>
#include <string>
#include <thread>
#include <vector>

BEGIN_MISC_NAMESPACE

class ChildProcess
{
public:
  ChildProcess(const std::string& processPath,
               const std::vector<std::string>& args);

  ~ChildProcess();

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

  int m_pid = 0;
};

END_MISC_NAMESPACE
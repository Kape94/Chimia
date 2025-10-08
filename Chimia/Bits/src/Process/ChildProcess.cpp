#include "Process/ChildProcess.h"

#include "ProcessUtils.h"

// ----------------------------------------------------------------------------

USING_BITS_NAMESPACE

// ----------------------------------------------------------------------------

#ifdef __linux__

// ----------------------------------------------------------------------------
// Linux platform implementation
// ----------------------------------------------------------------------------

#include <sys/wait.h>
#include <unistd.h>

class ChildProcess::PlatformImpl
{
public:
  PlatformImpl(const std::string& processPath,
               const std::vector<std::string>& args,
               const ProcessExitFn& exitCallback,
               const ProcessExitFn& terminateCallback)
    : m_exitCallback(exitCallback)
    , m_teminateCallback(terminateCallback)
  {
    pid_t childId = fork();

    const bool isChild = childId == 0;
    if (isChild) {
      ExecuteChildProcess(processPath, args);
    } else if (childId > 0) {
      m_pid = childId;
    }
  }

  void ExecuteChildProcess(const std::string& processPath,
                           const std::vector<std::string>& args)
  {
    ProcessUtils::Args programArgs(args, processPath);

    execvp(processPath.c_str(), programArgs.Argv());
    perror("failed to start process");

    exit(1);
  }

  void WatchProcess()
  {
    int status;
    waitpid(m_pid, &status, 0);

    if (WIFEXITED(status)) {
      m_exitCallback(WEXITSTATUS(status));
      m_pid = 0;
    } else if (WIFSIGNALED(status)) {
      m_teminateCallback(WTERMSIG(status));
      m_pid = 0;
    }
  }

  void Terminate()
  {
    if (m_pid > 0) {
      kill(m_pid, SIGTERM);
    }
  }
  int GetPID() { return m_pid; }

private:
  int m_pid = 0;

  ProcessExitFn m_exitCallback;
  ProcessExitFn m_teminateCallback;
};

#else

// ----------------------------------------------------------------------------
// Other platform implementation
// ----------------------------------------------------------------------------

class ChildProcess::PlatformImpl
{
public:
  PlatformImpl(const std::string& processPath,
               const std::vector<std::string>& args,
               const ProcessExitFn& exitCallback,
               const ProcessExitFn& terminateCallback)
    : m_exitCallback(exitCallback)
    , m_terminateCallback(terminateCallback)
  {
  }

  void WatchProcess()
  {
    m_exitCallback(0);
    m_terminateCallback(0);
  }

  void Terminate() {}

  int GetPID() { return 0; }

private:
  ProcessExitFn m_exitCallback;
  ProcessExitFn m_terminateCallback;
};

#endif

// ----------------------------------------------------------------------------

ChildProcess::ChildProcess(const std::string& processPath,
                           const std::vector<std::string>& args)
  : m_impl(new PlatformImpl(
      processPath,
      args,
      [&](int status) {
        if (m_onExit != nullptr)
          m_onExit(status);
      },
      [&](int status) {
        if (m_onTerminate)
          m_onTerminate(status);
      }))
{
  m_processWatcher = std::thread([&]() { WatchProcess(); });
}

// ----------------------------------------------------------------------------

void
ChildProcess::WatchProcess()
{
  m_impl->WatchProcess();
}

// ----------------------------------------------------------------------------

ChildProcess::~ChildProcess()
{
  Terminate();
}

// ----------------------------------------------------------------------------

void
ChildProcess::Await()
{
  if (m_processWatcher.joinable()) {
    m_processWatcher.join();
  }
}

// ----------------------------------------------------------------------------

void
ChildProcess::Terminate()
{
  m_impl->Terminate();
  Await();
}

// ----------------------------------------------------------------------------

void
ChildProcess::OnExit(ProcessExitFn fn)
{
  m_onExit = fn;
}

// ----------------------------------------------------------------------------

void
ChildProcess::OnTerminated(ProcessExitFn fn)
{
  m_onTerminate = fn;
}

// ----------------------------------------------------------------------------

int
ChildProcess::GetPID() const
{
  return m_impl->GetPID();
}

// ----------------------------------------------------------------------------

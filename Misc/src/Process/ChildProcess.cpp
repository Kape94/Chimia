#include "Process/ChildProcess.h"

#include "ProcessUtils.h"

#include <sys/wait.h>
#include <unistd.h>

// ----------------------------------------------------------------------------

USING_MISC_NAMESPACE

// ----------------------------------------------------------------------------

ChildProcess::ChildProcess(const std::string& processPath,
                           const std::vector<std::string>& args)
{
  pid_t childId = fork();

  const bool isChild = childId == 0;
  if (isChild) {
    ExecuteChildProcess(processPath, args);
  } else if (childId > 0) {
    m_pid = childId;
    m_processWatcher = std::thread([&]() { WatchProcess(); });
  }
}

// ----------------------------------------------------------------------------

void
ChildProcess::ExecuteChildProcess(const std::string& processPath,
                                  const std::vector<std::string>& args)
{
  ProcessUtils::Args programArgs(args, processPath);

  execvp(processPath.c_str(), programArgs.Argv());
  perror("failed to start process");

  exit(1);
}

// ----------------------------------------------------------------------------

void
ChildProcess::WatchProcess()
{
  int status;
  waitpid(m_pid, &status, 0);

  if (WIFEXITED(status)) {
    if (m_onExit != nullptr) {
      m_onExit(WEXITSTATUS(status));
    }
    m_pid = 0;
  } else if (WIFSIGNALED(status)) {
    if (m_onTerminate != nullptr) {
      m_onTerminate(WTERMSIG(status));
    }
    m_pid = 0;
  }
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
  if (m_pid > 0) {
    kill(m_pid, SIGTERM);
    Await();
  }
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
  return m_pid;
}

// ----------------------------------------------------------------------------

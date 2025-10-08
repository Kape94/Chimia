#include "Process/ProcessInfo.h"

#include <unistd.h>

#include <filesystem>
#include <fstream>
#include <sstream>

// ----------------------------------------------------------------------------

USING_BITS_NAMESPACE

// ----------------------------------------------------------------------------
// ProcessInfoUtils
// ----------------------------------------------------------------------------

namespace ProcessInfoUtils {
std::string
GetNthTokenFromLine(std::string& line, const int n)
{
  std::istringstream iss(line);

  int field = 1;
  std::string token;
  for (int field = 1; field <= n; ++field) {
    iss >> token;
  }

  return token;
}

// ------------------------------------------------------------------------

long
ReadStartTicksFromStat(std::ifstream& statFile)
{
  std::string statLine;
  std::getline(statFile, statLine);

  constexpr int startTicksField = 22;
  const std::string startTicksToken =
    GetNthTokenFromLine(statLine, startTicksField);

  return std::stol(startTicksToken);
}
}

// ------------------------------------------------------------------------
// Platform specific implementation
// ------------------------------------------------------------------------

#ifdef __linux__

// ------------------------------------------------------------------------
// Linux platform implementation
// ------------------------------------------------------------------------

class ProcessInfo::PlatformImpl
{
public:
  PlatformImpl(const int pid)
    : m_pid(pid)
  {
    std::filesystem::path processDir = ProcessDirectory();
    if (!std::filesystem::exists(processDir)) {
      Invalidate();
    } else {
      m_processTimestamp = ReadTimestampFromStat();
    }
  }

  bool IsRunning() const
  {
    if (!IsValid())
      return false;

    const double timestamp = GetTimestamp();
    return timestamp == m_processTimestamp;
  }

  std::string GetName() const
  {
    if (!IsValid())
      return "";

    std::ifstream comm(ProcessDirectory() + "comm");
    if (!comm.good())
      return "";

    std::string name;
    std::getline(comm, name);

    return name;
  }

  double GetTimestamp() const
  {
    if (!IsValid())
      return 0.0;

    return ReadTimestampFromStat();
  }

private:
  double ReadTimestampFromStat() const
  {
    std::ifstream stat(ProcessDirectory() + "stat");
    if (!stat.good())
      return 0;

    const long startTicks = ProcessInfoUtils::ReadStartTicksFromStat(stat);
    const long ticksPerSecond = sysconf(_SC_CLK_TCK);

    const double startSeconds =
      static_cast<double>(startTicks) / static_cast<double>(ticksPerSecond);
    return startSeconds;
  }

  std::string ProcessDirectory() const
  {
    const std::string pidStr = std::to_string(m_pid);
    return "/proc/" + pidStr + "/";
  }

  bool IsValid() const { return m_pid > 0 && m_processTimestamp > 0; }

  void Invalidate()
  {
    m_pid = 0;
    m_processTimestamp = 0;
  }

  double m_processTimestamp = 0.0;
  int m_pid = 0;
};

#else

// ------------------------------------------------------------------------
// Other platform implementation
// ------------------------------------------------------------------------

class ProcessInfo::PlatformImpl
{
public:
  PlatformImpl(const int) {}

  bool IsRunning() const { return false; }

  std::string GetName() const { return ""; }

  double GetTimestamp() const { return 0.0; }
};

#endif

// ----------------------------------------------------------------------------
// ProcessInfo
// ----------------------------------------------------------------------------

ProcessInfo::ProcessInfo(const int pid)
  : m_impl(new PlatformImpl(pid))
{
}

// ----------------------------------------------------------------------------

ProcessInfo::~ProcessInfo() = default;

// ----------------------------------------------------------------------------

bool
ProcessInfo::IsRunning() const
{
  return m_impl->IsRunning();
}

// ----------------------------------------------------------------------------

std::string
ProcessInfo::GetName() const
{
  return m_impl->GetName();
}

// ----------------------------------------------------------------------------

double
ProcessInfo::GetTimestamp() const
{
  return m_impl->GetTimestamp();
}

// ----------------------------------------------------------------------------

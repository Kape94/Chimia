#include "Process/ProcessInfo.h"

#include <unistd.h>

#include <filesystem>
#include <fstream>
#include <sstream>

// ----------------------------------------------------------------------------

USING_MISC_NAMESPACE

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

// ----------------------------------------------------------------------------
// ProcessInfo
// ----------------------------------------------------------------------------

ProcessInfo::ProcessInfo(const int pid)
  : m_pid(pid)
{
  std::filesystem::path processDir = ProcessDirectory();
  if (!std::filesystem::exists(processDir)) {
    Invalidate();
  } else {
    m_processTimestamp = ReadTimestampFromStat();
  }
}

// ----------------------------------------------------------------------------

bool
ProcessInfo::IsRunning() const
{
  if (!IsValid())
    return false;

  const double timestamp = GetTimestamp();
  return timestamp == m_processTimestamp;
}

// ----------------------------------------------------------------------------

std::string
ProcessInfo::GetName() const
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

// ----------------------------------------------------------------------------

double
ProcessInfo::GetTimestamp() const
{
  if (!IsValid())
    return 0.0;

  return ReadTimestampFromStat();
}

// ----------------------------------------------------------------------------

double
ProcessInfo::ReadTimestampFromStat() const
{
  std::ifstream stat(ProcessDirectory() + "stat");
  if (!stat.good())
    return 0;

  const long startTicks = ProcessInfoUtils::ReadStartTicksFromStat(stat);
  const long ticksPerSecond = sysconf(_SC_CLK_TCK);

  const double startSeconds = static_cast<double>(startTicks) / static_cast<double>(ticksPerSecond);
  return startSeconds;
}

// ----------------------------------------------------------------------------

std::string
ProcessInfo::ProcessDirectory() const
{
  const std::string pidStr = std::to_string(m_pid);
  return "/proc/" + pidStr + "/";
}

// ----------------------------------------------------------------------------

bool
ProcessInfo::IsValid() const
{
  return m_pid > 0 && m_processTimestamp > 0;
}

// ----------------------------------------------------------------------------

void
ProcessInfo::Invalidate()
{
  m_pid = 0;
  m_processTimestamp = 0;
}

// ----------------------------------------------------------------------------

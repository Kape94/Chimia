#pragma once

#include "Common/MiscNamespaceDefs.h"

#include <string>
#include <vector>

BEGIN_MISC_NAMESPACE

namespace ProcessUtils {

class Args
{
public:
  Args(const std::vector<std::string>& args, const std::string& processPath)
    : m_processPath(processPath)
    , m_args(args)
    , m_argc(m_args.size() + 2)
    , m_argv(m_argc, nullptr)
  {
    m_argv[0] = const_cast<char*>(m_processPath.c_str());
    for (int i = 0; i < m_args.size(); ++i) {
      m_argv[i + 1] = const_cast<char*>(m_args[i].c_str());
    }
    m_argv[m_argc - 1] = nullptr;
  }

  size_t Argc() { return m_argc; }
  char** Argv() { return m_argv.data(); }

private:
  std::string m_processPath;
  std::vector<std::string> m_args;

  const size_t m_argc;
  std::vector<char*> m_argv;
};

}

END_MISC_NAMESPACE
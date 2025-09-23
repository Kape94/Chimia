#pragma once

#include <thread>

#include "Misc/Common/MiscNamespaceDefs.h"

//-----------------------------------------------------------------------------

BEGIN_MISC_NAMESPACE

//-----------------------------------------------------------------------------

class AutoJoiningThread
{
public:
  AutoJoiningThread() = default;

  template<class Function, class... Args>
  AutoJoiningThread(Function f, Args... args)
    : thread(f, std::forward<Args>(args)...)
  {
  }

  ~AutoJoiningThread() { Join(); }

  AutoJoiningThread(const AutoJoiningThread& other) = delete;

  AutoJoiningThread& operator=(const AutoJoiningThread& other) = delete;

  AutoJoiningThread(AutoJoiningThread&& other)
    : thread(std::move(other.thread))
  {
  }

  AutoJoiningThread& operator=(AutoJoiningThread&& other)
  {
    thread = std::move(other.thread);
    return *this;
  }

  void Join()
  {
    if (thread.joinable())
      thread.join();
  }

private:
  std::thread thread;
};

//-----------------------------------------------------------------------------

END_MISC_NAMESPACE

//-----------------------------------------------------------------------------

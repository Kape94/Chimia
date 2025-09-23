#pragma once

#include "Misc/Common/MiscNamespaceDefs.h"
#include "Misc/Thread/AutoJoiningThread.h"

//-----------------------------------------------------------------------------

BEGIN_MISC_NAMESPACE

//-----------------------------------------------------------------------------

class Worker
{
public:
  Worker() = default;

  virtual ~Worker() = default;

  Worker(const Worker& other) = delete;

  Worker& operator=(const Worker& other) = delete;

  Worker(Worker&& other)
    : thread(std::move(other.thread))
  {
  }

  Worker& operator=(Worker&& other)
  {
    thread = std::move(other.thread);
    return *this;
  }

  void Start()
  {
    thread = AutoJoiningThread([&]() { Run(); });
  }

  virtual void Run() = 0;

private:
  AutoJoiningThread thread;
};

//-----------------------------------------------------------------------------

END_MISC_NAMESPACE

//-----------------------------------------------------------------------------

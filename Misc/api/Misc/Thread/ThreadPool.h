#pragma once

#include <vector>

#include "Misc/Common/MiscNamespaceDefs.h"

#include "Misc/Thread/ThreadPoolWorker.h"

//-----------------------------------------------------------------------------

BEGIN_MISC_NAMESPACE

//-----------------------------------------------------------------------------

class ThreadPool
{
public:
  ThreadPool() = delete;

  ThreadPool(const size_t _poolSize);

  ~ThreadPool();

  void PushTask(TaskQueue::Task task);

private:
  void Finish();

  const size_t poolSize = 0;

  std::vector<ThreadPoolWorker> workers;
  TaskQueue tasks;
};

//-----------------------------------------------------------------------------

END_MISC_NAMESPACE

//-----------------------------------------------------------------------------

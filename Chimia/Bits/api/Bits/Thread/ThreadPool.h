#pragma once

#include <vector>

#include "Bits/Common/BitsNamespaceDefs.h"

#include "Bits/Thread/ThreadPoolWorker.h"

//-----------------------------------------------------------------------------

BEGIN_BITS_NAMESPACE

//-----------------------------------------------------------------------------

class ThreadPool
{
public:
  ThreadPool() = delete;

  ThreadPool(const size_t _poolSize);

  ~ThreadPool();

  ThreadPool(const ThreadPool& other) = delete;
  ThreadPool& operator=(const ThreadPool& other) = delete;

  ThreadPool(ThreadPool&& other) = delete;
  ThreadPool& operator=(ThreadPool&& other) = delete;

  void PushTask(TaskQueue::Task task);

private:
  void Finish();

  const size_t poolSize = 0;

  std::vector<ThreadPoolWorker> workers;
  TaskQueue tasks;
};

//-----------------------------------------------------------------------------

END_BITS_NAMESPACE

//-----------------------------------------------------------------------------

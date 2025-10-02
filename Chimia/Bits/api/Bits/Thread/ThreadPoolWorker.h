#pragma once

#include "Bits/Common/BitsNamespaceDefs.h"
#include "Bits/Thread/Worker.h"

#include "Bits/Thread/TaskQueue.h"

//-----------------------------------------------------------------------------

BEGIN_BITS_NAMESPACE

//-----------------------------------------------------------------------------

class ThreadPoolWorker : public Worker
{
public:
  ThreadPoolWorker(TaskQueue& _tasks);

  virtual ~ThreadPoolWorker() = default;

  ThreadPoolWorker(const ThreadPoolWorker&) = delete;

  ThreadPoolWorker& operator=(const ThreadPoolWorker& other) = delete;

  ThreadPoolWorker(ThreadPoolWorker&& other);

  ThreadPoolWorker& operator=(ThreadPoolWorker&& other);

  void Finish();

private:
  void Run() override;

  bool keepRunning = true;

  TaskQueue& tasks;
};

//-----------------------------------------------------------------------------

END_BITS_NAMESPACE

//-----------------------------------------------------------------------------

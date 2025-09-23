#ifndef MISC_THREAD_POOL_WORKER_H
#define MISC_THREAD_POOL_WORKER_H

#include "Misc/Common/MiscNamespaceDefs.h"
#include "Misc/Thread/TaskQueue.h"
#include "Misc/Thread/Worker.h"

//-----------------------------------------------------------------------------

BEGIN_MISC_NAMESPACE

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

END_MISC_NAMESPACE

//-----------------------------------------------------------------------------

#endif

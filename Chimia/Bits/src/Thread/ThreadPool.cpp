#include "Thread/ThreadPool.h"

using namespace std::literals::chrono_literals;

//-----------------------------------------------------------------------------

USING_BITS_NAMESPACE

//-----------------------------------------------------------------------------

ThreadPool::ThreadPool(const size_t _poolSize)
  : poolSize(_poolSize)
{
  workers.reserve(poolSize);
  for (size_t i = 0; i < poolSize; ++i) {
    workers.emplace_back(tasks);
  }

  for (ThreadPoolWorker& worker : workers) {
    worker.Start();
  }
}

//-----------------------------------------------------------------------------

ThreadPool::~ThreadPool()
{
  Finish();
}

//-----------------------------------------------------------------------------

void
ThreadPool::PushTask(TaskQueue::Task task)
{
  tasks.PushTask(task);
}

//-----------------------------------------------------------------------------

void
ThreadPool::Finish()
{
  while (tasks.NTasks() > 0) {
    std::this_thread::sleep_for(10ms);
  }

  for (ThreadPoolWorker& worker : workers) {
    worker.Finish();
  }
}

//-----------------------------------------------------------------------------

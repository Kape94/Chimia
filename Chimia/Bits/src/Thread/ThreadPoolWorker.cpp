#include "Bits/Thread/ThreadPoolWorker.h"

#include <thread>

using namespace std::literals::chrono_literals;

//-----------------------------------------------------------------------------

USING_BITS_NAMESPACE

//-----------------------------------------------------------------------------

ThreadPoolWorker::ThreadPoolWorker(TaskQueue& _tasks)
  : tasks(_tasks)
{
}

//-----------------------------------------------------------------------------

ThreadPoolWorker::ThreadPoolWorker(ThreadPoolWorker&& other)
  : Worker(std::move(other))
  , tasks(other.tasks)
{
}

//-----------------------------------------------------------------------------

ThreadPoolWorker&
ThreadPoolWorker::operator=(ThreadPoolWorker&& other)
{
  ThreadPoolWorker copy(std::move(other));
  std::swap(*this, copy);
  return *this;
}

//-----------------------------------------------------------------------------

void
ThreadPoolWorker::Run()
{
  while (keepRunning) {
    std::optional<TaskQueue::Task> taskOpt = tasks.PopTask();
    if (taskOpt.has_value()) {
      TaskQueue::Task& task = taskOpt.value();
      task();
    }

    std::this_thread::sleep_for(10ms);
  }
}

//-----------------------------------------------------------------------------

void
ThreadPoolWorker::Finish()
{
  keepRunning = false;
}

//-----------------------------------------------------------------------------

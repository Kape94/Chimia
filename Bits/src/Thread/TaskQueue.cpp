#include "TaskQueue.h"

//-----------------------------------------------------------------------------

USING_BITS_NAMESPACE

//-----------------------------------------------------------------------------

void
TaskQueue::PushTask(TaskQueue::Task task)
{
  std::lock_guard lock(queueMutex);
  tasks.push(task);
}

//-----------------------------------------------------------------------------

std::optional<TaskQueue::Task>
TaskQueue::PopTask()
{
  std::lock_guard lock(queueMutex);
  if (tasks.size() == 0) {
    return {};
  }

  Task selectedTask = tasks.front();
  tasks.pop();
  return selectedTask;
}

//-----------------------------------------------------------------------------

size_t
TaskQueue::NTasks()
{
  std::lock_guard lock(queueMutex);
  return tasks.size();
}

//-----------------------------------------------------------------------------

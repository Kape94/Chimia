#pragma once

#include <functional>
#include <mutex>
#include <optional>
#include <queue>

#include "Bits/Common/BitsNamespaceDefs.h"

//-----------------------------------------------------------------------------

BEGIN_BITS_NAMESPACE

//-----------------------------------------------------------------------------

class TaskQueue
{
public:
  TaskQueue() = default;

  TaskQueue(const TaskQueue& other) = delete;
  TaskQueue& operator=(const TaskQueue& other) = delete;

  TaskQueue(TaskQueue&& other) = delete;
  TaskQueue& operator=(TaskQueue&& other) = delete;

  using Task = std::function<void(void)>;

  void PushTask(Task task);

  std::optional<Task> PopTask();

  size_t NTasks();

private:
  std::mutex queueMutex;
  std::queue<Task> tasks;
};

//-----------------------------------------------------------------------------

END_BITS_NAMESPACE

//-----------------------------------------------------------------------------

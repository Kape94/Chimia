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

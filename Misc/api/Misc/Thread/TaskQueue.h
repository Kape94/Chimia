#pragma once

#include <functional>
#include <mutex>
#include <optional>
#include <queue>

#include "Misc/Common/MiscNamespaceDefs.h"

//-----------------------------------------------------------------------------

BEGIN_MISC_NAMESPACE

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

END_MISC_NAMESPACE

//-----------------------------------------------------------------------------

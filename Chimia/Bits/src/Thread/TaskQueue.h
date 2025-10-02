#ifndef MISC_TASK_QUEUE_H
#define MISC_TASK_QUEUE_H

#include <functional>
#include <mutex>
#include <optional>
#include <queue>

#include "Common/BitsNamespaceDefs.h"

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

#endif

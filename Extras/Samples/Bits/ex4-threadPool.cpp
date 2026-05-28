#include <iostream>
#include <thread>

#include <mutex>
#include <string>

#include "Bits/Thread/ThreadPool.h"

using namespace std::literals;

std::mutex ioMutex;

template<class InfoType>
void
PrintTaskProgress(const std::string& taskName, const InfoType& info)
{
  std::lock_guard lock(ioMutex);

  std::cout << taskName << ": " << info << std::endl;
}

int
main()
{
  Chimia::Bits::ThreadPool pool(4);

  pool.PushTask([]() {
    for (int i = 0; i < 10; ++i) {
      PrintTaskProgress("Task #1", i);
      std::this_thread::sleep_for(1s);
    }
  });

  pool.PushTask([]() {
    for (int i = 0; i < 10; ++i) {
      const char c = static_cast<char>(65 + i);
      PrintTaskProgress("Task #2", c);
      std::this_thread::sleep_for(750ms);
    }
  });

  return 0;
}
#include "Misc/Process/ChildProcess.h"
#include "Misc/Process/ProcessInfo.h"

#include <iostream>
#include <string>

int
main()
{
  Misc::ChildProcess process("sleep", { "10" });

  process.OnExit([](int status) {
    std::cout << "Process exited with code: " << status << std::endl;
  });

  process.OnTerminated([](int status) {
    std::cout << "Process terminated with code: " << status << std::endl;
  });

  std::cout << "Started process with PID: " << process.GetPID() << std::endl;

  std::cout << "Before ProcessInfo" << std::endl;
  Misc::ProcessInfo processInfo(process.GetPID());
  std::cout << "After ProcessInfo" << std::endl;

  Misc::ChildProcess otherprocess("sleep", { "1" });
  otherprocess.Await();

  std::cout << "Process " << std::endl;
  std::cout << "Process " << processInfo.GetName() << " is running?"
            << processInfo.IsRunning() << std::endl;

  process.Terminate();
  process.Await();

  std::cout << "Process is still running? " << processInfo.IsRunning()
            << std::endl;

  return 0;
}
#include "Misc/Process/ChildProcess.h"

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

  process.Await();

  return 0;
}
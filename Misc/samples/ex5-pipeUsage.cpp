#include "Misc/IPC/PipeHost.h"
#include "Misc/IPC/PipeWriter.h"

#include <iostream>
#include <string>
#include <thread>

using namespace std::chrono_literals;

int
main()
{
  const std::string pipePath("myPipe");

  Misc::PipeHost pipe(pipePath);

  pipe.OnDataReceived([](const std::string& message) {
    std::cout << "Read message: " << message << std::endl;
    std::cout << "Message size: " << message.size() << std::endl;
  });
  pipe.Listen();

  Misc::PipeWriter writer(pipePath);
  writer.Write("Hello");

  std::this_thread::sleep_for(2s);

  writer.Write("World");

  std::this_thread::sleep_for(2s);

  pipe.StopListening();

  writer.Write("One last thing");

  std::this_thread::sleep_for(1s);

  return 0;
}
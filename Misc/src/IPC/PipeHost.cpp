#include "IPC/PipeHost.h"

#include <iostream>
#include <thread>

// ----------------------------------------------------------------------------

using namespace std::chrono_literals;

// ----------------------------------------------------------------------------

USING_MISC_NAMESPACE

// ----------------------------------------------------------------------------

PipeHost::PipeHost(const std::string& path)
  : pipe(path)
{
}

// ----------------------------------------------------------------------------

PipeHost::~PipeHost()
{
  StopListening();
}

// ----------------------------------------------------------------------------

void
PipeHost::OnDataReceived(DataReceivedFn dataReceivedFunction)
{
  onDataReceived = dataReceivedFunction;
}

// ----------------------------------------------------------------------------

void
PipeHost::Listen()
{
  isListening = true;

  listeningThread = std::thread([&]() {
    std::cout << "Started listening" << std::endl;
    while (isListening) {
      const std::string data = pipe.Read();
      if (data != "" && onDataReceived != nullptr) {
        onDataReceived(data);
      }
      std::this_thread::sleep_for(10ms);
    }
    std::cout << "Stopped listening" << std::endl;
  });
}

// ----------------------------------------------------------------------------

void
PipeHost::StopListening()
{
  if (isListening) {
    isListening = false;
    listeningThread.join();
  }
}

// ----------------------------------------------------------------------------

std::string
PipeHost::Read()
{
  return pipe.Read();
}

// ----------------------------------------------------------------------------

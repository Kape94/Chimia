#include "IPC/PipeHost.h"

#include <iostream>
#include <thread>

// ----------------------------------------------------------------------------

using namespace std::chrono_literals;

// ----------------------------------------------------------------------------

USING_BITS_NAMESPACE

// ----------------------------------------------------------------------------
// Linux platform implementation
// ----------------------------------------------------------------------------

#ifdef __linux__

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

class PipeHost::PlatformImpl
{
public:
  PlatformImpl(const std::string& _path)
    : path(_path)
  {
    Create();
  }

  ~PlatformImpl()
  {
    if (fdRead != -1) {
      close(fdRead);
    }
    Close();
  }

  void Create()
  {
    if (mkfifo(path.c_str(), 0666) == -1) {
      std::cout << "Error while creating the pipe" << std::endl;
    }

    fdRead = open(path.c_str(), O_RDONLY | O_NONBLOCK);
    if (fdRead == -1) {
      std::cout << "Error while opening the pipe for reading" << std::endl;
    }
  }

  void Close() { const int status = unlink(path.c_str()); }

  std::string Read()
  {
    char buffer[2048];
    ssize_t bytesRead = read(fdRead, buffer, sizeof(buffer));
    if (bytesRead == -1) {
      return std::string();
    }

    return buffer;
  }

private:
  const std::string path;
  int fdRead = -1;
};

#else

// ----------------------------------------------------------------------------
// Other platform implementation
// ----------------------------------------------------------------------------

class PipeHost::PlatformImpl
{
public:
  PlatformImpl(const std::string& _path) { Create(); }

  ~PlatformImpl() { Close(); }

  void Create() {}

  void Close() {}

  std::string Read() { return ""; }
};

#endif

// ----------------------------------------------------------------------------

PipeHost::PipeHost(const std::string& path)
  : m_impl(new PlatformImpl(path))
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
      const std::string data = Read();
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
  return m_impl->Read();
}

// ----------------------------------------------------------------------------

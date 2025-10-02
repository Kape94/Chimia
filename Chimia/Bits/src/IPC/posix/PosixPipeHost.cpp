#include "IPC/posix/PosixPipeHost.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

// ----------------------------------------------------------------------------

USING_BITS_NAMESPACE

// ----------------------------------------------------------------------------

PosixPipeHost::PosixPipeHost(const std::string& _path)
  : path(_path)
{
  Create();
}

// ----------------------------------------------------------------------------

PosixPipeHost::~PosixPipeHost()
{
  if (fdRead != -1) {
    close(fdRead);
  }
  Close();
}

// ----------------------------------------------------------------------------

void
PosixPipeHost::Create()
{
  if (mkfifo(path.c_str(), 0666) == -1) {
    std::cout << "Error while creating the pipe" << std::endl;
  }

  fdRead = open(path.c_str(), O_RDONLY | O_NONBLOCK);
  if (fdRead == -1) {
    std::cout << "Error while opening the pipe for reading" << std::endl;
  }
}

// ----------------------------------------------------------------------------

void
PosixPipeHost::Close()
{
  const int status = unlink(path.c_str());
}

// ----------------------------------------------------------------------------

std::string
PosixPipeHost::Read()
{
  char buffer[2048];
  ssize_t bytesRead = read(fdRead, buffer, sizeof(buffer));
  if (bytesRead == -1) {
    return std::string();
  }

  return buffer;
}

// ----------------------------------------------------------------------------

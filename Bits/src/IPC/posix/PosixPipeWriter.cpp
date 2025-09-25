#include "IPC/posix/PosixPipeWriter.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

// ----------------------------------------------------------------------------

USING_BITS_NAMESPACE

// ----------------------------------------------------------------------------

PosixPipeWriter::PosixPipeWriter(const std::string& _path)
  : path(_path)
{
  fdWrite = open(path.c_str(), O_WRONLY);
  if (fdWrite == -1) {
    std::cout << "Error while opening the pipe for writing" << std::endl;
  }
}

// ----------------------------------------------------------------------------

PosixPipeWriter::~PosixPipeWriter()
{
  if (fdWrite != -1) {
    close(fdWrite);
  }
}

// ----------------------------------------------------------------------------

void
PosixPipeWriter::Write(const std::string& message)
{
  if (fdWrite == -1) {
    fdWrite = open(path.c_str(), O_WRONLY);
    if (fdWrite == -1) {
      std::cout << "Error while opening the pipe for writing" << std::endl;
    }
  }

  if (write(fdWrite, message.c_str(), message.size()) == -1) {
    std::cout << "Error while writing" << std::endl;
  }
}

// ----------------------------------------------------------------------------

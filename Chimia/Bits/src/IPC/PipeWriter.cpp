#include "IPC/PipeWriter.h"

// ----------------------------------------------------------------------------

USING_BITS_NAMESPACE

// ----------------------------------------------------------------------------

#ifdef __linux__

// ----------------------------------------------------------------------------
// Linux platform implementation
// ----------------------------------------------------------------------------

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

class PipeWriter::PlatformImpl
{

public:
  PlatformImpl(const std::string& _path)
    : path(_path)
  {
    fdWrite = open(path.c_str(), O_WRONLY);
    if (fdWrite == -1) {
      std::cout << "Error while opening the pipe for writing" << std::endl;
    }
  }

  ~PlatformImpl()
  {
    if (fdWrite != -1) {
      close(fdWrite);
    }
  }

  void Write(const std::string& message)
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

private:
  const std::string path;
  int fdWrite = -1;
};

#else

// ----------------------------------------------------------------------------
// Other platform implementation
// ----------------------------------------------------------------------------

class PipeWriter::PlatformImpl
{

public:
  PlatformImpl(const std::string& _path) {}

  ~PlatformImpl() {}

  void Write(const std::string& message) {}
};

#endif

// ----------------------------------------------------------------------------

PipeWriter::PipeWriter(const std::string& path)
  : m_impl(new PlatformImpl(path))
{
}

// ----------------------------------------------------------------------------

PipeWriter::~PipeWriter() {}

// ----------------------------------------------------------------------------

void
PipeWriter::Write(const std::string& message)
{
  return m_impl->Write(message);
}

// ----------------------------------------------------------------------------

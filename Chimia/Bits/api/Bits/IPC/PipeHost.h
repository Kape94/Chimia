#pragma once

#include "Bits/Common/BitsNamespaceDefs.h"

#include "Bits/IPC/posix/PosixPipeHost.h"

#include <functional>
#include <string>
#include <thread>

BEGIN_BITS_NAMESPACE

class PipeHost
{
public:
  PipeHost(const std::string& path);

  ~PipeHost();

  void Listen();
  void StopListening();

  using DataReceivedFn = std::function<void(const std::string&)>;
  void OnDataReceived(DataReceivedFn dataReceivedFunction);

private:
  std::string Read();

  bool isListening = false;
  DataReceivedFn onDataReceived = nullptr;
  std::thread listeningThread;
  Chimia::Bits::PosixPipeHost pipe;
};

END_BITS_NAMESPACE

#pragma once

#include "Bits/Common/BitsNamespaceDefs.h"

#include <functional>
#include <memory>
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

  class PlatformImpl;
  std::unique_ptr<PlatformImpl> m_impl;

  bool isListening = false;
  DataReceivedFn onDataReceived = nullptr;
  std::thread listeningThread;
};

END_BITS_NAMESPACE

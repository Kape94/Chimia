#include "Diagnostics.h"

#include <cstdlib>
#include <iostream>

// ----------------------------------------------------------------------------

USING_CHIMIA_CORE_NAMESPACE

// ----------------------------------------------------------------------------

namespace DiagnosticsInternal {
auto defaultErrorHandler = [](const int errorCode, const std::string& message) {
  std::cout << "Error " << errorCode << ":\n" << message << "\n";
  exit(errorCode);
};

std::function<void(const int, const std::string&)> onError =
  defaultErrorHandler;
}

// ----------------------------------------------------------------------------

void
Diagnostics::Error(const int errorCode, const std::string& message)
{
  DiagnosticsInternal::onError(errorCode, message);
}

// ----------------------------------------------------------------------------

void
Diagnostics::OnError(
  const std::function<void(const int, const std::string&)>& onErrorFunction)
{
  DiagnosticsInternal::onError = onErrorFunction;
}

// ----------------------------------------------------------------------------
#pragma once

#include "DiagnosticsNamespaceDefs.h"

#include <functional>
#include <string>

BEGIN_CHIMIA_DIAGNOSTICS_NAMESPACE

void
OnError(
  const std::function<void(const int, const std::string&)>& onErrorFunction);

void
Error(const int errorCode, const std::string& message);

END_CHIMIA_DIAGNOSTICS_NAMESPACE
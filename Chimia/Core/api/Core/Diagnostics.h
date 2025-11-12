#pragma once

#include "CoreNamespaceDefs.h"

#include <functional>
#include <string>

BEGIN_CHIMIA_CORE_NAMESPACE

namespace Diagnostics {

void
OnError(
  const std::function<void(const int, const std::string&)>& onErrorFunction);

void
Error(const int errorCode, const std::string& message);

}

END_CHIMIA_CORE_NAMESPACE
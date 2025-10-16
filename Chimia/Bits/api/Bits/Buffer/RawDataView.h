#pragma once

#include "Bits/Common/BitsNamespaceDefs.h"

#include <cstddef>

BEGIN_BITS_NAMESPACE

struct RawDataView
{
  const void* data;
  const size_t size;
};

END_BITS_NAMESPACE
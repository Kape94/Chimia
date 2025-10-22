#pragma once

#include "Bits/Common/BitsNamespaceDefs.h"

#include <cstddef>

BEGIN_BITS_NAMESPACE

struct RawDataView
{
  const void* data;
  const size_t size;
};

struct RawArrayView
{
  const void* array;
  const size_t nItems;
  const size_t itemSize;

  size_t TotalSize() const { return nItems * itemSize; }
};

END_BITS_NAMESPACE
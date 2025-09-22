#pragma once

#include "RenderLibNamespaceDefs.h"

#include "IndexedBuffer.h"

// --------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------

class BufferPrivate
{
public:
  static unsigned GetVAO(const Buffer& buffer);
  static unsigned GetVAO(const IndexedBuffer& buffer);
  static unsigned GetNElements(const IndexedBuffer& buffer);
};

// --------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------
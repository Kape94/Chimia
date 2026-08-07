#pragma once

#include "RenderingNamespaceDefs.h"

#include "ShaderBinding.h"

// --------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------

namespace BufferUtils {

unsigned
CreateBufferAndLoadData(const void* data,
                        const unsigned dataSize,
                        const bool isIndexBuffer);

void
LoadDataOnBuffer(const unsigned bufferID,
                 const void* data,
                 const unsigned dataSize,
                 const bool isIndexBuffer);

void
LinkShaderBinding(const ShaderBinding& binding);

}

// --------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------
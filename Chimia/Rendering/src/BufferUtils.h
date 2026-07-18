#pragma once

#include "RenderingNamespaceDefs.h"

#include "ShaderAttribute.h"

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
LinkShaderAttributes(const ShaderAttributes& shaderAttributes);

void
LinkShaderAttribute(const ShaderAttribute& attr,
                    const unsigned offset,
                    const unsigned totalAttributeSize);

unsigned
ComputeTotalSizeOfAttributes(const ShaderAttributes& attrs);

unsigned
ComputeTotalEntriesOfAttributes(const ShaderAttributes& attrs);

void
LinkInstancedShaderAttributes(const ShaderAttributes& attrs);

}

// --------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------
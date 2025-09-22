#pragma once

#include "RenderLibNamespaceDefs.h"

#include "ShaderAttribute.h"

// --------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------

namespace BufferUtils {

unsigned
CreateBufferAndLoadData(const unsigned bufferType,
                        const void* data,
                        const unsigned dataSize);

void
LoadDataOnBuffer(const unsigned bufferID,
                 const unsigned bufferType,
                 const void* data,
                 const unsigned dataSize);

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
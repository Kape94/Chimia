#pragma once

#include "InstancedDataBuffer.h"
#include "RenderingNamespaceDefs.h"

#include "ShaderAttribute.h"
#include "ShaderBinding.h"
#include "VertexData.h"

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
LinkShaderAttributes(const ShaderAttributes& shaderAttributes,
                     const VertexData& vertexData);

void
LinkShaderAttributes(const ShaderAttributes& shaderAttributes,
                     const InstancedDataBuffer& instancedData);

unsigned
ComputeTotalSizeOfAttributes(const ShaderAttributes& attrs);

void
LinkShaderBinding(const ShaderBinding& binding);

}

// --------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------
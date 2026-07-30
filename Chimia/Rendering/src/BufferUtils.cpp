#include "BufferUtils.h"

#include "BufferPrivate.h"
#include "GLState.h"
#include "InstancedData.h"
#include "OpenGLDefs.h"
#include "ShaderBinding.h"
#include "VertexData.h"

// --------------------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------
// Helper functions
// --------------------------------------------------------------------------------------

namespace {

void
BindBuffer(const unsigned id, const bool isIndexBuffer)
{
  if (isIndexBuffer) {
    GLState::BindElementBuffer(id);
  } else {
    GLState::BindArrayBuffer(id);
  }
}

int
GetGLBufferType(const bool isIndexBuffer)
{
  return isIndexBuffer ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER;
}

void
BindDataFromShaderBinding(const ShaderBinding& binding)
{
  const VertexDataInstance& vertexData = BufferPrivate::GetVertexData(binding);
  const InstancedDataInstance& instancedData =
    BufferPrivate::GetInstancedData(binding);

  if (vertexData != nullptr) {
    BufferPrivate::Bind(vertexData);
  } else {
    BufferPrivate::Bind(instancedData);
  }
}

unsigned
CalculateShaderBindingStride(const ShaderBinding& binding)
{
  const VertexDataInstance& vertexData = BufferPrivate::GetVertexData(binding);
  const InstancedDataInstance& instancedData =
    BufferPrivate::GetInstancedData(binding);

  return vertexData != nullptr ? BufferPrivate::GetLayoutSize(vertexData)
                               : BufferPrivate::GetInstanceSize(instancedData);
}

void
SetAttributeRateFromBinding(const ShaderBinding& binding)
{
  const InstancedDataInstance& instancedData =
    BufferPrivate::GetInstancedData(binding);

  const bool isInstanced = instancedData != nullptr;
  if (isInstanced) {
    const unsigned location = BufferPrivate::GetLocation(binding);
    glVertexAttribDivisor(location, 1);
  }
}

}

// --------------------------------------------------------------------------------------
// BufferUtils
// --------------------------------------------------------------------------------------

unsigned
BufferUtils::CreateBufferAndLoadData(const void* data,
                                     const unsigned dataSize,
                                     const bool isIndexBuffer)
{
  unsigned newVBO;
  glGenBuffers(1, &newVBO);
  BindBuffer(newVBO, isIndexBuffer);

  const int bufferType = GetGLBufferType(isIndexBuffer);
  glBufferData(bufferType, dataSize, data, GL_STATIC_DRAW);

  return newVBO;
}

//---------------------------------------------------------------------------------------

void
BufferUtils::LoadDataOnBuffer(const unsigned bufferID,
                              const void* data,
                              const unsigned dataSize,
                              const bool isIndexBuffer)
{
  BindBuffer(bufferID, isIndexBuffer);

  const int bufferType = GetGLBufferType(isIndexBuffer);
  glBufferSubData(bufferType, 0, dataSize, data);
}

//---------------------------------------------------------------------------------------

void
BufferUtils::LinkShaderBinding(const ShaderBinding& binding)
{
  BindDataFromShaderBinding(binding);

  const unsigned location = BufferPrivate::GetLocation(binding);
  const unsigned nEntries = BufferPrivate::GetNEntries(binding);
  const unsigned dataType = BufferPrivate::GetDataType(binding);
  const unsigned offset = BufferPrivate::GetOffset(binding);
  const unsigned stride = CalculateShaderBindingStride(binding);

  glVertexAttribPointer(location,
                        nEntries,
                        dataType,
                        GL_FALSE,
                        stride,
                        reinterpret_cast<void*>(offset));

  glEnableVertexAttribArray(location);

  SetAttributeRateFromBinding(binding);
}

//---------------------------------------------------------------------------------------
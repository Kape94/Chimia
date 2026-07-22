#include "BufferUtils.h"

#include "BufferPrivate.h"
#include "GLState.h"
#include "InstancedDataBuffer.h"
#include "OpenGLDefs.h"
#include "ShaderAttribute.h"
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
BufferUtils::LinkShaderAttributes(const ShaderAttributes& shaderAttributes)
{
  const unsigned totalVertexSize =
    ComputeTotalSizeOfAttributes(shaderAttributes);

  unsigned offset = 0;
  for (const ShaderAttribute& attribute : shaderAttributes) {
    LinkShaderAttribute(attribute, offset, totalVertexSize);

    offset += attribute.DataSizeInBytes();
  }
}

//---------------------------------------------------------------------------------------

void
BufferUtils::LinkShaderAttribute(const ShaderAttribute& attr,
                                 const unsigned offset,
                                 const unsigned totalAttributeSize)
{
  const unsigned location = attr.Location();

  glVertexAttribPointer(location,
                        attr.NEntries(),
                        attr.DataType(),
                        GL_FALSE,
                        totalAttributeSize,
                        reinterpret_cast<void*>(offset));

  glEnableVertexAttribArray(location);
}

//---------------------------------------------------------------------------------------

unsigned
BufferUtils::ComputeTotalSizeOfAttributes(const ShaderAttributes& attrs)
{
  unsigned totalSize = 0;
  for (const ShaderAttribute& attr : attrs) {
    totalSize += attr.DataSizeInBytes();
  }
  return totalSize;
}

//---------------------------------------------------------------------------------------

unsigned
BufferUtils::ComputeTotalEntriesOfAttributes(const ShaderAttributes& attrs)
{
  unsigned totalEntries = 0;
  for (const ShaderAttribute& attr : attrs) {
    totalEntries += attr.NEntries();
  }
  return totalEntries;
}

//---------------------------------------------------------------------------------------

void
BufferUtils::LinkInstancedShaderAttributes(const ShaderAttributes& attrs)
{
  LinkShaderAttributes(attrs);
  for (const ShaderAttribute& attr : attrs) {
    glVertexAttribDivisor(attr.Location(), 1);
  }
}

//---------------------------------------------------------------------------------------

void
BufferUtils::LinkShaderBinding(const ShaderBinding& binding)
{
  const VertexData* vertexData = BufferPrivate::GetVertexData(binding);
  const InstancedDataBuffer* instancedData =
    BufferPrivate::GetInstancedData(binding);

  const bool isInstanced = instancedData != nullptr;

  if (vertexData != nullptr) {
    BufferPrivate::Bind(*vertexData);
  } else {
    BufferPrivate::Bind(*instancedData);
  }

  const unsigned location = BufferPrivate::GetLocation(binding);
  const unsigned nEntries = BufferPrivate::GetNEntries(binding);
  const unsigned dataType = BufferPrivate::GetDataType(binding);
  const unsigned offset = BufferPrivate::GetOffset(binding);

  const unsigned stride = vertexData != nullptr
                            ? BufferPrivate::GetLayoutSize(*vertexData)
                            : BufferPrivate::GetInstanceSize(*instancedData);

  glVertexAttribPointer(location,
                        nEntries,
                        dataType,
                        GL_FALSE,
                        stride,
                        reinterpret_cast<void*>(offset));

  glEnableVertexAttribArray(location);

  if (isInstanced) {
    glVertexAttribDivisor(location, 1);
  }
}

//---------------------------------------------------------------------------------------
#include "BufferUtils.h"

#include "OpenGLDefs.h"
#include "ShaderAttribute.h"

// --------------------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------

unsigned
BufferUtils::CreateBufferAndLoadData(const unsigned bufferType,
                                     const void* data,
                                     const unsigned dataSize)
{
  unsigned newVBO;
  glGenBuffers(1, &newVBO);
  glBindBuffer(bufferType, newVBO);

  glBufferData(bufferType, dataSize, data, GL_STATIC_DRAW);

  return newVBO;
}

//---------------------------------------------------------------------------------------

void
BufferUtils::LoadDataOnBuffer(const unsigned bufferID,
                              const unsigned bufferType,
                              const void* data,
                              const unsigned dataSize)
{
  glBindBuffer(bufferType, bufferID);
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

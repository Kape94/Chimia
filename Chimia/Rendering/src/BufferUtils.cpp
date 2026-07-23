#include "BufferUtils.h"

#include "BufferPrivate.h"
#include "GLState.h"
#include "InstancedData.h"
#include "OpenGLDefs.h"
#include "ShaderAttribute.h"
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
  const VertexData* vertexData = BufferPrivate::GetVertexData(binding);
  const InstancedData* instancedData = BufferPrivate::GetInstancedData(binding);

  if (vertexData != nullptr) {
    BufferPrivate::Bind(*vertexData);
  } else {
    BufferPrivate::Bind(*instancedData);
  }
}

unsigned
CalculateShaderBindingStride(const ShaderBinding& binding)
{
  const VertexData* vertexData = BufferPrivate::GetVertexData(binding);
  const InstancedData* instancedData = BufferPrivate::GetInstancedData(binding);

  return vertexData != nullptr ? BufferPrivate::GetLayoutSize(*vertexData)
                               : BufferPrivate::GetInstanceSize(*instancedData);
}

void
SetAttributeRateFromBinding(const ShaderBinding& binding)
{
  const InstancedData* instancedData = BufferPrivate::GetInstancedData(binding);
  const unsigned location = BufferPrivate::GetLocation(binding);

  const bool isInstanced = instancedData != nullptr;
  if (isInstanced) {
    glVertexAttribDivisor(location, 1);
  }
}

template<class InputData>
ShaderBinding
CreateShaderBinding(const ShaderAttribute& attribute,
                    const unsigned offset,
                    const InputData& data)
{
  const unsigned type = attribute.DataType();
  const unsigned location = attribute.Location();
  const unsigned nEntries = attribute.NEntries();

  switch (type) {
    case GL_DOUBLE:
      return ShaderBinding::Double(data, location, nEntries, offset);
    case GL_INT:
      return ShaderBinding::Int(data, location, nEntries, offset);
    case GL_UNSIGNED_INT:
      return ShaderBinding::UnsignedInt(data, location, nEntries, offset);
    case GL_SHORT:
      return ShaderBinding::Short(data, location, nEntries, offset);
    case GL_UNSIGNED_SHORT:
      return ShaderBinding::UnsignedShort(data, location, nEntries, offset);
    case GL_BYTE:
      return ShaderBinding::Byte(data, location, nEntries, offset);
    case GL_UNSIGNED_BYTE:
      return ShaderBinding::UnsignedByte(data, location, nEntries, offset);
    case GL_FLOAT:
    default:
      return ShaderBinding::Float(data, location, nEntries, offset);
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
BufferUtils::LinkShaderAttributes(const ShaderAttributes& shaderAttributes,
                                  const VertexData& vertexData)
{
  std::vector<ShaderBinding> bindings;

  unsigned offset = 0;
  for (const ShaderAttribute& attribute : shaderAttributes) {
    bindings.emplace_back(CreateShaderBinding(attribute, offset, vertexData));
    offset += attribute.DataSizeInBytes();
  }

  for (const ShaderBinding& binding : bindings) {
    LinkShaderBinding(binding);
  }
}

//---------------------------------------------------------------------------------------

void
BufferUtils::LinkShaderAttributes(const ShaderAttributes& shaderAttributes,
                                  const InstancedData& instancedData)
{
  std::vector<ShaderBinding> bindings;

  unsigned offset = 0;
  for (const ShaderAttribute& attribute : shaderAttributes) {
    bindings.emplace_back(
      CreateShaderBinding(attribute, offset, instancedData));
    offset += attribute.DataSizeInBytes();
  }

  for (const ShaderBinding& binding : bindings) {
    LinkShaderBinding(binding);
  }
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
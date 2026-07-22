#include "ShaderBinding.h"

#include "OpenGLDefs.h"

//-----------------------------------------------------------------------------

namespace ShaderBindingUtils {
unsigned
sizeOfDataType(const unsigned dataType)
{
  switch (dataType) {
    case GL_FLOAT:
      return sizeof(float);
    case GL_DOUBLE:
      return sizeof(double);
    case GL_INT:
      return sizeof(int);
    case GL_UNSIGNED_INT:
      return sizeof(unsigned int);
    case GL_SHORT:
      return sizeof(short);
    case GL_UNSIGNED_SHORT:
      return sizeof(unsigned short);
    case GL_BYTE:
      return sizeof(char);
    case GL_UNSIGNED_BYTE:
      return sizeof(unsigned char);
    default:
      return sizeof(float);
  }
}
}

//-----------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

//-----------------------------------------------------------------------------

ShaderBinding::ShaderBinding(const VertexData* vertexData,
                             const InstancedDataBuffer* instancedData,
                             const unsigned location,
                             const unsigned nEntries,
                             const unsigned dataType,
                             const unsigned offset)
  : m_referenceVertexData(vertexData)
  , m_referenceInstancedData(instancedData)
  , m_location(location)
  , m_nEntries(nEntries)
  , m_dataType(dataType)
  , m_dataSizeInBytes(ShaderBindingUtils::sizeOfDataType(dataType) * nEntries)
  , m_offset(offset)
{
}

//-----------------------------------------------------------------------------

ShaderBinding::ShaderBinding(const ShaderBinding& other)
  : m_referenceVertexData(other.m_referenceVertexData)
  , m_referenceInstancedData(other.m_referenceInstancedData)
  , m_location(other.m_location)
  , m_nEntries(other.m_nEntries)
  , m_dataType(other.m_dataType)
  , m_dataSizeInBytes(other.m_dataSizeInBytes)
  , m_offset(other.m_offset)
{
}

//-----------------------------------------------------------------------------

ShaderBinding&
ShaderBinding::operator=(const ShaderBinding& other)
{
  m_referenceVertexData = other.m_referenceVertexData;
  m_referenceInstancedData = other.m_referenceInstancedData;
  m_location = other.m_location;
  m_nEntries = other.m_nEntries;
  m_dataType = other.m_dataType;
  m_dataSizeInBytes = other.m_dataSizeInBytes;
  m_offset = other.m_offset;

  return *this;
}

//-----------------------------------------------------------------------------

ShaderBinding
ShaderBinding::Float(const VertexData& data,
                     const unsigned location,
                     const unsigned nEntries,
                     const unsigned offset)
{
  return ShaderBinding(
    &data, nullptr /*instancedData*/, location, nEntries, GL_FLOAT, offset);
}

//-----------------------------------------------------------------------------

ShaderBinding
ShaderBinding::Double(const VertexData& data,
                      const unsigned location,
                      const unsigned nEntries,
                      const unsigned offset)
{
  return ShaderBinding(
    &data, nullptr /*instancedData*/, location, nEntries, GL_DOUBLE, offset);
}

//-----------------------------------------------------------------------------

ShaderBinding
ShaderBinding::Int(const VertexData& data,
                   const unsigned location,
                   const unsigned nEntries,
                   const unsigned offset)
{
  return ShaderBinding(
    &data, nullptr /*instancedData*/, location, nEntries, GL_INT, offset);
}

//-----------------------------------------------------------------------------

ShaderBinding
ShaderBinding::UnsignedInt(const VertexData& data,
                           const unsigned location,
                           const unsigned nEntries,
                           const unsigned offset)
{
  return ShaderBinding(&data,
                       nullptr /*instancedData*/,
                       location,
                       nEntries,
                       GL_UNSIGNED_INT,
                       offset);
}

//-----------------------------------------------------------------------------

ShaderBinding
ShaderBinding::Short(const VertexData& data,
                     const unsigned location,
                     const unsigned nEntries,
                     const unsigned offset)
{
  return ShaderBinding(
    &data, nullptr /*instancedData*/, location, nEntries, GL_SHORT, offset);
}

//-----------------------------------------------------------------------------

ShaderBinding
ShaderBinding::UnsignedShort(const VertexData& data,
                             const unsigned location,
                             const unsigned nEntries,
                             const unsigned offset)
{
  return ShaderBinding(&data,
                       nullptr /*instancedData*/,
                       location,
                       nEntries,
                       GL_UNSIGNED_SHORT,
                       offset);
}

//-----------------------------------------------------------------------------

ShaderBinding
ShaderBinding::Byte(const VertexData& data,
                    const unsigned location,
                    const unsigned nEntries,
                    const unsigned offset)
{
  return ShaderBinding(
    &data, nullptr /*instancedData*/, location, nEntries, GL_BYTE, offset);
}

//-----------------------------------------------------------------------------

ShaderBinding
ShaderBinding::UnsignedByte(const VertexData& data,
                            const unsigned location,
                            const unsigned nEntries,
                            const unsigned offset)
{
  return ShaderBinding(&data,
                       nullptr /*instancedData*/,
                       location,
                       nEntries,
                       GL_UNSIGNED_BYTE,
                       offset);
}

//-----------------------------------------------------------------------------

ShaderBinding
ShaderBinding::Float(const InstancedDataBuffer& data,
                     const unsigned location,
                     const unsigned nEntries,
                     const unsigned offset)
{
  return ShaderBinding(
    nullptr /*vertexData*/, &data, location, nEntries, GL_FLOAT, offset);
}

//-----------------------------------------------------------------------------

ShaderBinding
ShaderBinding::Double(const InstancedDataBuffer& data,
                      const unsigned location,
                      const unsigned nEntries,
                      const unsigned offset)
{
  return ShaderBinding(
    nullptr /*vertexData*/, &data, location, nEntries, GL_DOUBLE, offset);
}

//-----------------------------------------------------------------------------

ShaderBinding
ShaderBinding::Int(const InstancedDataBuffer& data,
                   const unsigned location,
                   const unsigned nEntries,
                   const unsigned offset)
{
  return ShaderBinding(
    nullptr /*vertexData*/, &data, location, nEntries, GL_INT, offset);
}

//-----------------------------------------------------------------------------

ShaderBinding
ShaderBinding::UnsignedInt(const InstancedDataBuffer& data,
                           const unsigned location,
                           const unsigned nEntries,
                           const unsigned offset)
{
  return ShaderBinding(
    nullptr /*vertexData*/, &data, location, nEntries, GL_UNSIGNED_INT, offset);
}

//-----------------------------------------------------------------------------

ShaderBinding
ShaderBinding::Short(const InstancedDataBuffer& data,
                     const unsigned location,
                     const unsigned nEntries,
                     const unsigned offset)
{
  return ShaderBinding(
    nullptr /*vertexData*/, &data, location, nEntries, GL_SHORT, offset);
}

//-----------------------------------------------------------------------------

ShaderBinding
ShaderBinding::UnsignedShort(const InstancedDataBuffer& data,
                             const unsigned location,
                             const unsigned nEntries,
                             const unsigned offset)
{
  return ShaderBinding(nullptr /*vertexData*/,
                       &data,
                       location,
                       nEntries,
                       GL_UNSIGNED_SHORT,
                       offset);
}

//-----------------------------------------------------------------------------

ShaderBinding
ShaderBinding::Byte(const InstancedDataBuffer& data,
                    const unsigned location,
                    const unsigned nEntries,
                    const unsigned offset)
{
  return ShaderBinding(
    nullptr /*vertexData*/, &data, location, nEntries, GL_BYTE, offset);
}

//-----------------------------------------------------------------------------

ShaderBinding
ShaderBinding::UnsignedByte(const InstancedDataBuffer& data,
                            const unsigned location,
                            const unsigned nEntries,
                            const unsigned offset)
{
  return ShaderBinding(nullptr /*vertexData*/,
                       &data,
                       location,
                       nEntries,
                       GL_UNSIGNED_BYTE,
                       offset);
}

//-----------------------------------------------------------------------------

const VertexData*
ShaderBinding::GetVertexData() const
{
  return m_referenceVertexData;
}

//-----------------------------------------------------------------------------

const InstancedDataBuffer*
ShaderBinding::GetInstancedData() const
{
  return m_referenceInstancedData;
}

//-----------------------------------------------------------------------------

unsigned
ShaderBinding::GetLocation() const
{
  return m_location;
}

//-----------------------------------------------------------------------------

unsigned
ShaderBinding::GetNEntries() const
{
  return m_nEntries;
}

//-----------------------------------------------------------------------------

unsigned
ShaderBinding::GetDataType() const
{
  return m_dataType;
}

//-----------------------------------------------------------------------------

unsigned
ShaderBinding::GetOffset() const
{
  return m_offset;
}

//-----------------------------------------------------------------------------
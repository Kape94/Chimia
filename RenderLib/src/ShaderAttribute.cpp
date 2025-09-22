#include "ShaderAttribute.h"

#include "OpenGLDefs.h"

namespace ShaderAttributeUtils {
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

ShaderAttribute::ShaderAttribute(const unsigned _location,
                                 const unsigned _nEntries,
                                 const unsigned _dataType)
  : location(_location)
  , nEntries(_nEntries)
  , dataType(_dataType)
  , dataSizeInBytes(ShaderAttributeUtils::sizeOfDataType(_dataType) * _nEntries)
{
}

//-----------------------------------------------------------------------------

ShaderAttribute
ShaderAttribute::Float(const unsigned location, const unsigned nEntries)
{
  return ShaderAttribute(location, nEntries, GL_FLOAT);
}

//-----------------------------------------------------------------------------

ShaderAttribute
ShaderAttribute::Double(const unsigned location, const unsigned nEntries)
{
  return ShaderAttribute(location, nEntries, GL_DOUBLE);
}

//-----------------------------------------------------------------------------

ShaderAttribute
ShaderAttribute::Int(const unsigned location, const unsigned nEntries)
{
  return ShaderAttribute(location, nEntries, GL_INT);
}

//-----------------------------------------------------------------------------

ShaderAttribute
ShaderAttribute::UnsignedInt(const unsigned location, const unsigned nEntries)
{
  return ShaderAttribute(location, nEntries, GL_UNSIGNED_INT);
}

//-----------------------------------------------------------------------------

ShaderAttribute
ShaderAttribute::Short(const unsigned location, const unsigned nEntries)
{
  return ShaderAttribute(location, nEntries, GL_SHORT);
}

//-----------------------------------------------------------------------------

ShaderAttribute
ShaderAttribute::UnsignedShort(const unsigned location, const unsigned nEntries)
{
  return ShaderAttribute(location, nEntries, GL_UNSIGNED_SHORT);
}

//-----------------------------------------------------------------------------

ShaderAttribute
ShaderAttribute::Byte(const unsigned location, const unsigned nEntries)
{
  return ShaderAttribute(location, nEntries, GL_BYTE);
}

//-----------------------------------------------------------------------------

ShaderAttribute
ShaderAttribute::UnsignedByte(const unsigned location, const unsigned nEntries)
{
  return ShaderAttribute(location, nEntries, GL_UNSIGNED_BYTE);
}

//-----------------------------------------------------------------------------

unsigned
ShaderAttribute::Location() const
{
  return location;
}

//-----------------------------------------------------------------------------

unsigned
ShaderAttribute::NEntries() const
{
  return nEntries;
}

//-----------------------------------------------------------------------------

unsigned
ShaderAttribute::DataType() const
{
  return dataType;
}

//-----------------------------------------------------------------------------

unsigned
ShaderAttribute::DataSizeInBytes() const
{
  return dataSizeInBytes;
}

//-----------------------------------------------------------------------------

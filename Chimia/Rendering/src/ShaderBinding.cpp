#include "ShaderBinding.h"

#include "BufferPrivate.h"
#include "DataLayout.h"
#include "DataLayoutUtils.h"
#include "InstancedData.h"
#include "OpenGLDefs.h"

//-----------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

//-----------------------------------------------------------------------------

namespace ShaderBindingUtils {

int
GLTypeForData(const eDataType dataType)
{
  switch (dataType) {
    case eDataType::FLOAT:
    case eDataType::VECTOR_2_FLOAT:
    case eDataType::VECTOR_3_FLOAT:
    case eDataType::VECTOR_4_FLOAT:
    case eDataType::MATRIX_FLOAT_4X4:
      return GL_FLOAT;
    case eDataType::DOUBLE:
      return GL_DOUBLE;
    case eDataType::INT:
      return GL_INT;
    case eDataType::UNSIGNED_INT:
      return GL_UNSIGNED_INT;
    case eDataType::BOOL:
      return GL_BYTE;
  }
}

struct DecomposedAttribute
{
  std::vector<std::pair<unsigned, unsigned>> locationsAndEntries;
  size_t entrySize = 0;
};

DecomposedAttribute
DecomposeLocationAndEntries(const eDataType dataType,
                            const unsigned baseLocation)
{
  switch (dataType) {
    case eDataType::FLOAT:
    case eDataType::DOUBLE:
    case eDataType::INT:
    case eDataType::UNSIGNED_INT:
    case eDataType::BOOL:
      return { { { baseLocation, 1 } },
               DataLayoutUtils::DataTypeSize(dataType) };
    case eDataType::VECTOR_2_FLOAT:
      return { { { baseLocation, 2 } },
               DataLayoutUtils::DataTypeSize(eDataType::FLOAT) };
    case eDataType::VECTOR_3_FLOAT:
      return { { { baseLocation, 3 } },
               DataLayoutUtils::DataTypeSize(eDataType::FLOAT) };
    case eDataType::VECTOR_4_FLOAT:
      return { { { baseLocation, 4 } },
               DataLayoutUtils::DataTypeSize(eDataType::FLOAT) };
    case eDataType::MATRIX_FLOAT_4X4:
      return { { { baseLocation, 4 },
                 { baseLocation + 1, 4 },
                 { baseLocation + 2, 4 },
                 { baseLocation + 3, 4 } },
               DataLayoutUtils::DataTypeSize(eDataType::FLOAT) };
  }
}

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
// ShaderBinding
//-----------------------------------------------------------------------------

ShaderBinding::ShaderBinding(const VertexDataInstance& vertexData,
                             const InstancedDataInstance& instancedData,
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

ShaderBindings
ShaderBinding::Connect(const VertexDataInstance& data,
                       const std::string& vertexAttribute,
                       const Shader& shader,
                       const std::string& shaderAttribute)
{
  const DataLayout& vertexLayout = BufferPrivate::GetDataLayout(data);
  const eDataType vertexType = vertexLayout.DataTypeOf(vertexAttribute);

  const unsigned baseLocation =
    BufferPrivate::GetAttributeLocation(shaderAttribute, shader);
  const unsigned baseOffset = vertexLayout.OffsetOf(vertexAttribute);
  const int dataType = ShaderBindingUtils::GLTypeForData(vertexType);

  const auto decomposedAttributes =
    ShaderBindingUtils::DecomposeLocationAndEntries(vertexType, baseLocation);

  ShaderBindings bindings;

  unsigned offset = baseOffset;
  for (const auto& locationAndEntry :
       decomposedAttributes.locationsAndEntries) {
    const unsigned location = locationAndEntry.first;
    const unsigned nEntries = locationAndEntry.second;

    bindings.Insert(
      ShaderBinding(data, nullptr, location, nEntries, dataType, offset));
    offset += decomposedAttributes.entrySize * nEntries;
  }

  return bindings;
}

//-----------------------------------------------------------------------------

ShaderBindings
ShaderBinding::Connect(const InstancedDataInstance& data,
                       const std::string& vertexAttribute,
                       const Shader& shader,
                       const std::string& shaderAttribute)
{
  const DataLayout& vertexLayout = BufferPrivate::GetDataLayout(data);
  const eDataType vertexType = vertexLayout.DataTypeOf(vertexAttribute);

  const unsigned baseLocation =
    BufferPrivate::GetAttributeLocation(shaderAttribute, shader);
  const unsigned baseOffset = vertexLayout.OffsetOf(vertexAttribute);
  const int dataType = ShaderBindingUtils::GLTypeForData(vertexType);

  const auto decomposedAttributes =
    ShaderBindingUtils::DecomposeLocationAndEntries(vertexType, baseLocation);

  ShaderBindings bindings;

  unsigned offset = baseOffset;
  for (const auto& locationAndEntry :
       decomposedAttributes.locationsAndEntries) {
    const unsigned location = locationAndEntry.first;
    const unsigned nEntries = locationAndEntry.second;

    bindings.Insert(
      ShaderBinding(nullptr, data, location, nEntries, dataType, offset));
    offset += decomposedAttributes.entrySize * nEntries;
  }

  return bindings;
}

//-----------------------------------------------------------------------------

const VertexDataInstance&
ShaderBinding::GetVertexData() const
{
  return m_referenceVertexData;
}

//-----------------------------------------------------------------------------

const InstancedDataInstance&
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
// ShaderBindings
//-----------------------------------------------------------------------------

ShaderBindings::ShaderBindings(
  const std::initializer_list<ShaderBindings>& bindingsList)
{
  for (const auto& bindings : bindingsList) {
    m_bindings.insert(m_bindings.end(), bindings.begin(), bindings.end());
  }
}

//-----------------------------------------------------------------------------

ShaderBindings::ShaderBindings(
  const std::initializer_list<ShaderBinding>& bindings)
{
  m_bindings.insert(m_bindings.end(), bindings.begin(), bindings.end());
}

//-----------------------------------------------------------------------------

void
ShaderBindings::Insert(const ShaderBinding& binding)
{
  m_bindings.push_back(binding);
}

//-----------------------------------------------------------------------------

void
ShaderBindings::Insert(const ShaderBindings& bindings)
{
  m_bindings.insert(m_bindings.end(), bindings.begin(), bindings.end());
}

//-----------------------------------------------------------------------------

void
ShaderBindings::Clear()
{
  m_bindings.clear();
}

//-----------------------------------------------------------------------------

std::vector<ShaderBinding>::const_iterator
ShaderBindings::begin() const
{
  return m_bindings.begin();
}

//-----------------------------------------------------------------------------

std::vector<ShaderBinding>::const_iterator
ShaderBindings::end() const
{
  return m_bindings.end();
}

//-----------------------------------------------------------------------------

#include "InstancedData.h"

#include "BufferUtils.h"
#include "Core/Types.h"
#include "GLState.h"
#include "OpenGLDefs.h"

// ----------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

std::shared_ptr<InstancedData>
InstancedData::New()
{
  return std::shared_ptr<InstancedData>(new InstancedData);
}

// ----------------------------------------------------------------------------

InstancedData::InstancedData(InstancedData&& other)
  : m_instancedVBO(other.m_instancedVBO)
  , m_nInstances(other.m_nInstances)
  , m_currentSize(other.m_currentSize)
  , m_maximumSize(other.m_maximumSize)
{
  other.m_instancedVBO = 0;
  other.m_nInstances = 0;
  other.m_currentSize = 0;
  other.m_maximumSize = 0;
}

// ----------------------------------------------------------------------------

InstancedData&
InstancedData::operator=(InstancedData&& other)
{
  m_instancedVBO = other.m_instancedVBO;
  m_nInstances = other.m_nInstances;
  m_currentSize = other.m_currentSize;
  m_maximumSize = other.m_maximumSize;

  other.m_instancedVBO = 0;
  other.m_nInstances = 0;
  other.m_currentSize = 0;
  other.m_maximumSize = 0;

  return *this;
}

// ----------------------------------------------------------------------------

InstancedData::~InstancedData()
{
  Clear();
}

// ----------------------------------------------------------------------------

void
InstancedData::Create(const RawArrayView& instancedData)
{
  Clear();

  const void* instancedRawData = instancedData.array;
  const unsigned instanceDataSize = instancedData.itemSize;
  const unsigned nInstances = instancedData.nItems;

  m_instancedVBO = BufferUtils::CreateBufferAndLoadData(
    instancedRawData, instanceDataSize * nInstances, false /*isIndexBuffer*/);

  m_nInstances = nInstances;

  const size_t totalDataSize = instancedData.TotalSize();
  m_currentSize = totalDataSize;
  m_maximumSize = totalDataSize;
}

// ----------------------------------------------------------------------------

void
InstancedData::Load(const RawArrayView& instancedData)
{
  const size_t incomingSize = instancedData.TotalSize();
  if (m_instancedVBO == 0 || incomingSize > m_maximumSize) {
    return;
  }

  BufferUtils::LoadDataOnBuffer(
    m_instancedVBO, instancedData.array, incomingSize, false /*isIndexBuffer*/);

  m_nInstances = instancedData.nItems;
  m_currentSize = incomingSize;
}

// ----------------------------------------------------------------------------

void
InstancedData::Clear()
{
  if (m_instancedVBO != 0) {
    glDeleteBuffers(1, &m_instancedVBO);
    m_instancedVBO = 0;
  }

  m_nInstances = 0;
}

// ----------------------------------------------------------------------------

void
InstancedData::Bind() const
{
  if (m_instancedVBO != 0) {
    GLState::BindArrayBuffer(m_instancedVBO);
  }
}

// ----------------------------------------------------------------------------

unsigned
InstancedData::GetNInstances() const
{
  return m_nInstances;
}

// ----------------------------------------------------------------------------

unsigned
InstancedData::GetInstanceSize() const
{
  return static_cast<unsigned>(m_currentSize) / m_nInstances;
}

// ----------------------------------------------------------------------------
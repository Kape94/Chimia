#include "InstancedDataBuffer.h"

#include "BufferUtils.h"
#include "Core/Types.h"
#include "OpenGLDefs.h"

// ----------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

InstancedDataBuffer::InstancedDataBuffer(InstancedDataBuffer&& other)
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

InstancedDataBuffer&
InstancedDataBuffer::operator=(InstancedDataBuffer&& other)
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

InstancedDataBuffer::~InstancedDataBuffer()
{
  Clear();
}

// ----------------------------------------------------------------------------

void
InstancedDataBuffer::Create(const RawArrayView& instancedData)
{
  Clear();

  const void* instancedRawData = instancedData.array;
  const unsigned instanceDataSize = instancedData.itemSize;
  const unsigned nInstances = instancedData.nItems;

  m_instancedVBO = BufferUtils::CreateBufferAndLoadData(
    GL_ARRAY_BUFFER, instancedRawData, instanceDataSize * nInstances);

  m_nInstances = nInstances;

  const size_t totalDataSize = instancedData.TotalSize();
  m_currentSize = totalDataSize;
  m_maximumSize = totalDataSize;
}

// ----------------------------------------------------------------------------

void
InstancedDataBuffer::Load(const RawArrayView& instancedData)
{
  const size_t incomingSize = instancedData.TotalSize();
  if (m_instancedVBO == 0 || incomingSize > m_maximumSize) {
    return;
  }

  BufferUtils::LoadDataOnBuffer(
    m_instancedVBO, GL_ARRAY_BUFFER, instancedData.array, incomingSize);

  m_nInstances = instancedData.nItems;
  m_currentSize = incomingSize;
}

// ----------------------------------------------------------------------------

void
InstancedDataBuffer::Clear()
{
  if (m_instancedVBO != 0) {
    glDeleteBuffers(1, &m_instancedVBO);
    m_instancedVBO = 0;
  }

  m_nInstances = 0;
}

// ----------------------------------------------------------------------------

void
InstancedDataBuffer::Bind() const
{
  if (m_instancedVBO != 0) {
    glBindBuffer(GL_ARRAY_BUFFER, m_instancedVBO);
  }
}

// ----------------------------------------------------------------------------

unsigned
InstancedDataBuffer::GetNInstances() const
{
  return m_nInstances;
}

// ----------------------------------------------------------------------------
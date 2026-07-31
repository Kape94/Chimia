#include "InstancedData.h"

#include "BufferUtils.h"
#include "Core/Types.h"
#include "DataLayout.h"
#include "DataListeners.h"
#include "GLState.h"
#include "IDataChangeListener.h"
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

InstancedData::InstancedData()
  : m_listeners(new DataListeners)
{
}

// ----------------------------------------------------------------------------

InstancedData::InstancedData(InstancedData&& other)
  : m_instancedVBO(other.m_instancedVBO)
  , m_nInstances(other.m_nInstances)
  , m_dataLayout(std::move(other.m_dataLayout))
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
  m_dataLayout = std::move(other.m_dataLayout);
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
InstancedData::Create(const RawDataView& instancedData,
                      const DataLayout& dataLayout)
{
  Clear();

  const void* instancedRawData = instancedData.data;

  const size_t instanceSize = dataLayout.TotalSize();
  const size_t totalDataSize = instancedData.size;

  const unsigned instanceDataSize = instanceSize;
  const unsigned nInstances = totalDataSize / instanceSize;

  m_instancedVBO = BufferUtils::CreateBufferAndLoadData(
    instancedRawData, instanceDataSize * nInstances, false /*isIndexBuffer*/);

  m_nInstances = nInstances;
  m_dataLayout = dataLayout;

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
InstancedData::Resize(const RawDataView& data)
{
  const DataLayout backupLayout = m_dataLayout;

  Clear();
  Create(data, backupLayout);

  m_listeners->DataChanged();
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

const DataLayout&
InstancedData::GetDataLayout() const
{
  return m_dataLayout;
}

// ----------------------------------------------------------------------------

DataListeners&
InstancedData::GetListeners()
{
  return *m_listeners;
}

// ----------------------------------------------------------------------------

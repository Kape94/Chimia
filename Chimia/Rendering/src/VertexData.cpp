#include "VertexData.h"

#include "BufferUtils.h"
#include "Core/Types.h"
#include "DataLayout.h"
#include "DataListeners.h"
#include "GLState.h"
#include "OpenGLDefs.h"

// ----------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

VertexData::VertexData()
  : m_listeners(new DataListeners)
{
}

// ----------------------------------------------------------------------------

VertexData::VertexData(VertexData&& other)
  : m_VBO(other.m_VBO)
  , m_sizePerVertex(other.m_sizePerVertex)
  , m_nVertices(other.m_nVertices)
  , m_currentVertexSize(other.m_currentVertexSize)
  , m_maximumVertexSize(other.m_maximumVertexSize)
  , m_dataLayout(std::move(other.m_dataLayout))
  , m_listeners(std::move(other.m_listeners))
{
  other.m_VBO = 0;
  other.m_sizePerVertex = 0;
  other.m_nVertices = 0;
  other.m_currentVertexSize = 0;
  other.m_maximumVertexSize = 0;
}

// ----------------------------------------------------------------------------

VertexData&
VertexData::operator=(VertexData&& other)
{
  m_VBO = other.m_VBO;
  m_sizePerVertex = other.m_sizePerVertex;
  m_nVertices = other.m_nVertices;
  m_currentVertexSize = other.m_currentVertexSize;
  m_maximumVertexSize = other.m_maximumVertexSize;
  m_dataLayout = std::move(other.m_dataLayout);
  m_listeners = std::move(other.m_listeners);

  other.m_VBO = 0;
  other.m_sizePerVertex = 0;
  other.m_nVertices = 0;
  other.m_currentVertexSize = 0;
  other.m_maximumVertexSize = 0;

  return *this;
}

// ----------------------------------------------------------------------------

VertexData::~VertexData()
{
  Clear();
}

// ----------------------------------------------------------------------------

std::shared_ptr<VertexData>
VertexData::Create(const RawDataView& vertexData, const DataLayout& dataLayout)
{
  VertexDataInstance newData(new VertexData);

  newData->Setup(vertexData, dataLayout);

  return newData;
}

// ----------------------------------------------------------------------------

void
VertexData::Setup(const RawDataView& vertexData, const DataLayout& dataLayout)
{
  m_dataLayout = dataLayout;

  const size_t dataSize = vertexData.size;
  const size_t layoutSize = dataLayout.TotalSize();

  const unsigned nVertices = static_cast<unsigned>(dataSize / layoutSize);
  AllocateVertexData(vertexData, nVertices);
}

// ----------------------------------------------------------------------------

void
VertexData::AllocateVertexData(const RawDataView& vertexData,
                               const unsigned nVertices)
{
  const size_t vertexDataSize = vertexData.size;
  m_VBO = BufferUtils::CreateBufferAndLoadData(
    vertexData.data, vertexDataSize, false /*isIndexBuffer*/);

  m_nVertices = nVertices;
  m_sizePerVertex = vertexDataSize / nVertices;

  m_currentVertexSize = vertexDataSize;
  m_maximumVertexSize = vertexDataSize;
}

// ----------------------------------------------------------------------------

void
VertexData::Load(const RawDataView& data)
{
  const size_t vertexDataSize = data.size;
  if (m_VBO == 0 || vertexDataSize > m_maximumVertexSize) {
    return;
  }

  BufferUtils::LoadDataOnBuffer(
    m_VBO, data.data, vertexDataSize, false /*isIndexBuffer*/);
  m_nVertices = vertexDataSize / m_sizePerVertex;

  m_currentVertexSize = vertexDataSize;
}

// ----------------------------------------------------------------------------

void
VertexData::Resize(const RawDataView& data)
{
  const DataLayout backupLayout = m_dataLayout;

  Clear();
  Setup(data, backupLayout);

  m_listeners->DataChanged();
}

// ----------------------------------------------------------------------------

void
VertexData::Clear()
{
  if (m_VBO != 0) {
    glDeleteBuffers(1, &m_VBO);
    m_VBO = 0;
  }

  m_sizePerVertex = 0;
  m_nVertices = 0;
  m_currentVertexSize = 0;
  m_maximumVertexSize = 0;
}

// ----------------------------------------------------------------------------

void
VertexData::Bind() const
{
  if (m_VBO != 0) {
    GLState::BindArrayBuffer(m_VBO);
  }
}

// ----------------------------------------------------------------------------

unsigned
VertexData::GetNVertices() const
{
  return m_nVertices;
}

// ----------------------------------------------------------------------------

unsigned
VertexData::GetLayoutSize() const
{
  return m_sizePerVertex;
}

// ----------------------------------------------------------------------------

DataListeners&
VertexData::GetListeners()
{
  return *m_listeners;
}

// ----------------------------------------------------------------------------

const DataLayout&
VertexData::GetDataLayout() const
{
  return m_dataLayout;
}

// ----------------------------------------------------------------------------

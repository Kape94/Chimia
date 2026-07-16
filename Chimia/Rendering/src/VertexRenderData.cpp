#include "VertexRenderData.h"

#include "BufferPrivate.h"
#include "Core/Types.h"
#include "IndexData.h"

// ----------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

VertexRenderData::VertexRenderData(VertexRenderData&& other)
  : m_vertexData(std::move(other.m_vertexData))
  , m_indexData(std::move(other.m_indexData))
{
}

// ----------------------------------------------------------------------------

VertexRenderData&
VertexRenderData::operator=(VertexRenderData&& other)
{
  m_vertexData = std::move(other.m_vertexData);
  m_indexData = std::move(other.m_indexData);

  return *this;
}

// ----------------------------------------------------------------------------

VertexRenderData::~VertexRenderData()
{
  Clear();
}

// ----------------------------------------------------------------------------

void
VertexRenderData::Create(const RawDataView& vertexData,
                         const unsigned nVertices)
{
  Clear();

  AllocateVertexData(vertexData, nVertices);
}

// ----------------------------------------------------------------------------

void
VertexRenderData::Create(const RawDataView& vertexData,
                         const unsigned nVertices,
                         const RawArrayView& indexData)
{
  Clear();

  AllocateVertexData(vertexData, nVertices);
  AllocateIndexData(indexData);
}

// ----------------------------------------------------------------------------

void
VertexRenderData::AllocateVertexData(const RawDataView& vertexData,
                                     const unsigned nVertices)
{
  m_vertexData.Create(vertexData, nVertices);
}

// ----------------------------------------------------------------------------

void
VertexRenderData::AllocateIndexData(const RawArrayView& indexData)
{
  m_indexData.reset(new IndexData);
  m_indexData->Create(indexData);
}

// ----------------------------------------------------------------------------

void
VertexRenderData::LoadVertexData(const RawDataView& data)
{
  m_vertexData.Load(data);
}

// ----------------------------------------------------------------------------

void
VertexRenderData::LoadIndexData(const RawArrayView& indexData)
{
  if (m_indexData == nullptr) {
    return;
  }

  m_indexData->LoadIndexData(indexData);
}

// ----------------------------------------------------------------------------

void
VertexRenderData::Clear()
{
  m_vertexData.Clear();

  if (m_indexData != nullptr) {
    m_indexData->Clear();
    m_indexData.reset(nullptr);
  }
}

// ----------------------------------------------------------------------------

void
VertexRenderData::Bind() const
{
  BufferPrivate::Bind(m_vertexData);
  if (m_indexData != nullptr) {
    BufferPrivate::Bind(*m_indexData);
  }
}

// ----------------------------------------------------------------------------

unsigned
VertexRenderData::GetNVertices() const
{
  return BufferPrivate::GetNVertices(m_vertexData);
}

// ----------------------------------------------------------------------------

unsigned
VertexRenderData::GetNIndices() const
{
  return m_indexData != nullptr ? BufferPrivate::GetNIndices(*m_indexData) : 0;
}

// ----------------------------------------------------------------------------

bool
VertexRenderData::HasIndices() const
{
  return m_indexData != nullptr;
}

// ----------------------------------------------------------------------------

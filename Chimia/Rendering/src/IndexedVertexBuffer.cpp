#include "IndexedVertexBuffer.h"

#include "Core/Types.h"
#include "VertexRenderData.h"

// ----------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

IndexedVertexBuffer::IndexedVertexBuffer(IndexedVertexBuffer&& other)
  : m_buffer(std::move(other.m_buffer))
{
}

// ----------------------------------------------------------------------------

IndexedVertexBuffer&
IndexedVertexBuffer::operator=(IndexedVertexBuffer&& other)
{
  m_buffer = std::move(other.m_buffer);

  return *this;
}

// ----------------------------------------------------------------------------

IndexedVertexBuffer::~IndexedVertexBuffer()
{
  Clear();
}

// ----------------------------------------------------------------------------

void
IndexedVertexBuffer::Create(const RawDataView& vertexData,
                            const unsigned nVertices,
                            const RawArrayView& indexData)
{
  m_buffer.Create(vertexData, nVertices, indexData);
}

// ----------------------------------------------------------------------------

void
IndexedVertexBuffer::LoadVertexData(const RawDataView& data)
{
  m_buffer.LoadVertexData(data);
}

// ----------------------------------------------------------------------------

void
IndexedVertexBuffer::LoadIndexData(const RawArrayView& indexData)
{
  m_buffer.LoadIndexData(indexData);
}

// ----------------------------------------------------------------------------

void
IndexedVertexBuffer::Clear()
{
  m_buffer.Clear();
}

// ----------------------------------------------------------------------------

const VertexRenderData&
IndexedVertexBuffer::GetBaseBuffer() const
{
  return m_buffer;
}

// ----------------------------------------------------------------------------
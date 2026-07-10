#include "VertexBuffer.h"

#include "Core/Types.h"

// ----------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

VertexBuffer::VertexBuffer(VertexBuffer&& other)
  : m_buffer(std::move(other.m_buffer))
{
}

// ----------------------------------------------------------------------------

VertexBuffer&
VertexBuffer::operator=(VertexBuffer&& other)
{
  m_buffer = std::move(other.m_buffer);
  return *this;
}

// ----------------------------------------------------------------------------

VertexBuffer::~VertexBuffer()
{
  Clear();
}

// ----------------------------------------------------------------------------

void
VertexBuffer::Create(const RawDataView& vertexData, const unsigned nVertices)
{
  m_buffer.Create(vertexData, nVertices);
}

// ----------------------------------------------------------------------------

void
VertexBuffer::Load(const RawDataView& data)
{
  m_buffer.LoadVertexData(data);
}

// ----------------------------------------------------------------------------

void
VertexBuffer::Clear()
{
  m_buffer.Clear();
}

// ----------------------------------------------------------------------------

const GenericVertexBuffer&
VertexBuffer::BaseBuffer() const
{
  return m_buffer;
}

// ----------------------------------------------------------------------------

#include "IndexData.h"

#include "BufferUtils.h"
#include "Core/Types.h"
#include "GLState.h"
#include "OpenGLDefs.h"

// ----------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

std::shared_ptr<IndexData>
IndexData::New()
{
  return std::shared_ptr<IndexData>(new IndexData);
}

// ----------------------------------------------------------------------------

IndexData::IndexData(IndexData&& other)
  : m_EBO(other.m_EBO)
  , m_nIndices(other.m_nIndices)
  , m_currentIndexSize(other.m_currentIndexSize)
  , m_maximumIndexSize(other.m_maximumIndexSize)
{
  other.m_EBO = 0;
  other.m_nIndices = 0;
  other.m_currentIndexSize = 0;
  other.m_maximumIndexSize = 0;
}

// ----------------------------------------------------------------------------

IndexData&
IndexData::operator=(IndexData&& other)
{
  m_EBO = other.m_EBO;
  m_nIndices = other.m_nIndices;
  m_currentIndexSize = other.m_currentIndexSize;
  m_maximumIndexSize = other.m_maximumIndexSize;

  other.m_EBO = 0;
  other.m_nIndices = 0;
  other.m_currentIndexSize = 0;
  other.m_maximumIndexSize = 0;

  return *this;
}

// ----------------------------------------------------------------------------

IndexData::~IndexData()
{
  Clear();
}

// ----------------------------------------------------------------------------

void
IndexData::Create(const RawArrayView& indexData)
{
  Clear();

  AllocateIndexData(indexData);
}

// ----------------------------------------------------------------------------

void
IndexData::AllocateIndexData(const RawArrayView& indexData)
{
  const unsigned indexDataSize = indexData.TotalSize();
  m_EBO = BufferUtils::CreateBufferAndLoadData(
    indexData.array, indexDataSize, true /*isIndexBuffer*/);

  m_nIndices = indexData.nItems;

  m_currentIndexSize = indexDataSize;
  m_maximumIndexSize = indexDataSize;
}

// ----------------------------------------------------------------------------

void
IndexData::LoadIndexData(const RawArrayView& indexData)
{
  const size_t incomingSize = indexData.TotalSize();
  if (m_EBO == 0 || incomingSize > m_maximumIndexSize) {
    return;
  }

  const unsigned nIndexValues = indexData.nItems;
  BufferUtils::LoadDataOnBuffer(
    m_EBO, indexData.array, indexData.TotalSize(), true /*isIndexBuffer*/);

  m_nIndices = nIndexValues;
  m_currentIndexSize = incomingSize;
}

// ----------------------------------------------------------------------------

void
IndexData::Clear()
{
  if (m_EBO != 0) {
    glDeleteBuffers(1, &m_EBO);
    m_EBO = 0;
  }

  m_nIndices = 0;
  m_currentIndexSize = 0;
  m_maximumIndexSize = 0;
}

// ----------------------------------------------------------------------------

void
IndexData::Bind() const
{
  if (m_EBO != 0) {
    GLState::BindElementBuffer(m_EBO);
  }
}

// ----------------------------------------------------------------------------

unsigned
IndexData::GetNIndices() const
{
  return m_nIndices;
}

// ----------------------------------------------------------------------------

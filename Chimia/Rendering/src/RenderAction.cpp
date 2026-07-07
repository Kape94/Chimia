#include "RenderAction.h"

#include "BufferPrivate.h"
#include "BufferUtils.h"
#include "Core/Types.h"
#include "OpenGLDefs.h"
#include "ShaderAttribute.h"
#include "VertexBuffer.h"

USING_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

RenderAction::RenderAction(RenderAction&& other) noexcept
  : m_VAO(other.m_VAO)
  , m_ownBuffer(std::move(other.m_ownBuffer))
  , m_referenceBuffer(other.m_referenceBuffer)
{
  other.m_VAO = 0;
  other.m_referenceBuffer = nullptr;
}

//---------------------------------------------------------------------------------------

RenderAction&
RenderAction::operator=(RenderAction&& other) noexcept
{
  if (&other != this) {
    m_VAO = other.m_VAO;
    m_ownBuffer = std::move(other.m_ownBuffer);
    m_referenceBuffer = other.m_referenceBuffer;

    other.m_VAO = 0;
    other.m_referenceBuffer = nullptr;
  }

  return *this;
}

//---------------------------------------------------------------------------------------

RenderAction::RenderAction(const RawDataView& bufferData,
                           const ShaderAttributes& shaderAttributes)
{
  Create(bufferData, shaderAttributes);
}

//---------------------------------------------------------------------------------------

RenderAction::~RenderAction()
{
  Clear();
}

//---------------------------------------------------------------------------------------

void
RenderAction::Create(const VertexBuffer& reusableVertexBuffer,
                     const ShaderAttributes& shaderAttributes)
{
  Clear();

  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);

  m_referenceBuffer = &reusableVertexBuffer;

  BufferPrivate::Bind(reusableVertexBuffer);
  BufferUtils::LinkShaderAttributes(shaderAttributes);
}

//---------------------------------------------------------------------------------------

void
RenderAction::Create(const RawDataView& vertexData,
                     const ShaderAttributes& shaderAttributes)
{
  Clear();

  const size_t vertexDataSize = vertexData.size;
  const size_t sizePerVertex =
    BufferUtils::ComputeTotalSizeOfAttributes(shaderAttributes);
  const size_t nVertices = vertexDataSize / sizePerVertex;

  m_ownBuffer.reset(new VertexBuffer);
  m_ownBuffer->Create(vertexData, nVertices);

  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);

  BufferPrivate::Bind(*m_ownBuffer);
  BufferUtils::LinkShaderAttributes(shaderAttributes);
}

//---------------------------------------------------------------------------------------

void
RenderAction::Load(const RawDataView& vertexData)
{
  if (m_VAO == 0 || m_ownBuffer == nullptr) {
    return;
  }

  m_ownBuffer->Load(vertexData);
}

//---------------------------------------------------------------------------------------

unsigned
RenderAction::GetVAO() const
{
  return m_VAO;
}

//---------------------------------------------------------------------------------------

unsigned
RenderAction::GetNVertices() const
{
  const VertexBuffer& buffer =
    m_referenceBuffer != nullptr ? *m_referenceBuffer : *m_ownBuffer;

  return BufferPrivate::GetNVertices(buffer);
}

//---------------------------------------------------------------------------------------

void
RenderAction::Clear()
{
  if (m_VAO != 0) {
    glDeleteVertexArrays(1, &m_VAO);
    m_VAO = 0;
  }

  m_ownBuffer.reset(nullptr);
  m_referenceBuffer = nullptr;
}

//---------------------------------------------------------------------------------------

void
RenderAction::Render() const
{
  const int nVertices = GetNVertices();

  glBindVertexArray(m_VAO);
  glDrawArrays(GL_TRIANGLES, 0, nVertices);
}

//---------------------------------------------------------------------------------------

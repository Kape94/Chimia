#pragma once

// ----------------------------------------------------------------------------

#include "RenderingNamespaceDefs.h"
#include "ShaderAttribute.h"

// ----------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------

class ReusableVertexBufferObject
{
public:
  ReusableVertexBufferObject() = default;

  ReusableVertexBufferObject(const ReusableVertexBufferObject& other) = delete;
  ReusableVertexBufferObject& operator=(
    const ReusableVertexBufferObject& other) = delete;

  ReusableVertexBufferObject(ReusableVertexBufferObject&& other);
  ReusableVertexBufferObject& operator=(ReusableVertexBufferObject&& other);

  ~ReusableVertexBufferObject();

  void Create(const void* vertexData,
              const unsigned vertexDataSize,
              const ShaderAttributes& shaderAttributes);

  void Clear();

private:
  void Bind() const;
  unsigned GetNVertices() const;
  const ShaderAttributes& GetShaderAttributes() const;

  friend class BufferPrivate;

  unsigned m_VBO = 0;
  unsigned m_sizePerVertex = 0;
  unsigned m_nVertices = 0;
  ShaderAttributes m_shaderAttributes;
};

// ----------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// ----------------------------------------------------------------------------
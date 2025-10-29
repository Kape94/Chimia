#pragma once

#include <vector>

#include "RenderingNamespaceDefs.h"

#include "Buffer.h"
#include "ReusableIndexedVertexBufferObject.h"
#include "ShaderAttribute.h"

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

class IndexedBuffer
{
public:
  IndexedBuffer() = default;

  IndexedBuffer(const IndexedBuffer& other) = delete;
  IndexedBuffer& operator=(const IndexedBuffer& other) = delete;

  IndexedBuffer(IndexedBuffer&& other) noexcept;

  IndexedBuffer& operator=(IndexedBuffer&& other) noexcept;

  IndexedBuffer(const std::vector<float>& vertexData,
                const std::vector<unsigned>& indexData,
                const ShaderAttributes& shaderAttributes);

  IndexedBuffer(const void* vertexData,
                const unsigned vertexDataSize,
                const unsigned* indexData,
                const unsigned nIndexDataItems,
                const ShaderAttributes& shaderAttributes);

  ~IndexedBuffer();

  void Create(const ReusableIndexedVertexBufferObject& reusableVertexBuffer);

  void Create(const std::vector<float>& vertexData,
              const std::vector<unsigned>& indexData,
              const ShaderAttributes& shaderAttributes);

  void Create(const void* vertexData,
              const unsigned vertexDataSize,
              const unsigned* indexData,
              const unsigned nIndexDataItems,
              const ShaderAttributes& shaderAttributes);

  void LoadVertexData(const void* vertexData, const unsigned vertexDataSize);
  void LoadIndexData(const unsigned* indexData, const unsigned nIndexValues);

  void Clear();

  void Render() const;

private:
  void LoadIndexDataInGPU(const unsigned* indexData,
                          const unsigned nIndexDataItems);

  unsigned GetVAO() const;

  unsigned GetNElements() const;

  friend class BufferPrivate;

  Buffer m_baseBuffer;
  unsigned m_EBO = 0;

  unsigned m_nElements = 0;
};

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------
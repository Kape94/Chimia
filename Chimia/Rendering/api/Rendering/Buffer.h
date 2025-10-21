#pragma once

#include <vector>

#include "RenderingNamespaceDefs.h"

#include "ShaderAttribute.h"

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

class Buffer
{
public:
  Buffer() = default;

  Buffer(const Buffer& other) = delete;
  Buffer& operator=(const Buffer& other) = delete;

  Buffer(Buffer&& other) noexcept;

  Buffer& operator=(Buffer&& other) noexcept;

  Buffer(const std::vector<float>& vertexData,
         const ShaderAttributes& shaderAttributes);

  Buffer(const void* vertexData,
         const unsigned vertexDataSize,
         const ShaderAttributes& shaderAttributes);

  ~Buffer();

  void Create(const std::vector<float>& vertexData,
              const ShaderAttributes& shaderAttributes);

  void Create(const void* vertexData,
              const unsigned vertexDataSize,
              const ShaderAttributes& shaderAttributes);

  void Load(const void* vertexData, const unsigned vertexDataSize);

  void Clear();

  void Render() const;

private:
  void LoadDataInGPU(const void* vertexData, const unsigned vertexDataSize);

  void LoadVertexDataInGPU(const void* vertexData,
                           const unsigned vertexDataSize);

  unsigned GetVAO() const;
  unsigned GetNVertices() const;

  friend class BufferPrivate;

  unsigned m_VAO = 0;
  unsigned m_VBO = 0;

  unsigned m_sizePerVertex = 0;
  unsigned m_nVertices = 0;
};

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------
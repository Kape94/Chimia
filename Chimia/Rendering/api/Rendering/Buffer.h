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

  Buffer(const float* vertexData,
         const unsigned nVertexDataItems,
         const ShaderAttributes& shaderAttributes);

  ~Buffer();

  void Create(const std::vector<float>& vertexData,
              const ShaderAttributes& shaderAttributes);

  void Create(const float* vertexData,
              const unsigned nVertexDataItems,
              const ShaderAttributes& shaderAttributes);

  void Load(const float* vertexData, const unsigned nVertexDataItems);

  void Clear();

  void Render() const;

private:
  void LoadDataInGPU(const float* vertexData, const unsigned nVertexDataItems);

  void LoadVertexDataInGPU(const float* vertexData,
                           const unsigned nVertexDataItems);

  unsigned GetVAO() const;
  unsigned GetNVertices() const;

  friend class BufferPrivate;

  unsigned m_VAO = 0;
  unsigned m_VBO = 0;

  unsigned m_nVertices = 0;
};

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------
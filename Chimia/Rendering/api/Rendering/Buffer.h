#pragma once

#include "Core/ClassDefs.h"
#include "RenderingNamespaceDefs.h"

#include "ReusableVertexBufferObject.h"
#include "ShaderAttribute.h"

#include "Core/Types.h"

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

class Buffer
{
public:
  DEFAULT_CONSTUCTIBLE(Buffer)
  NON_COPYABLE(Buffer)

  Buffer(Buffer&& other) noexcept;
  Buffer& operator=(Buffer&& other) noexcept;

  Buffer(const RawDataView& vertexData,
         const ShaderAttributes& shaderAttributes);

  ~Buffer();

  void Create(const ReusableVertexBufferObject& reusableVertexBuffer,
              const ShaderAttributes& shaderAttributes);

  void Create(const RawDataView& vertexData,
              const ShaderAttributes& shaderAttributes);

  void Load(const RawDataView& vertexData);

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
#pragma once

#include "Core/ClassDefs.h"
#include "FrameBuffer.h"
#include "IDataChangeListener.h"
#include "IndexData.h"
#include "InstancedData.h"
#include "RenderingNamespaceDefs.h"
#include "ShaderBinding.h"
#include "Target.h"
#include "Texture2D.h"
#include "TextureUnit.h"
#include "VertexData.h"

// --------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------

class BufferPrivate
{
public:
  STATIC_CLASS(BufferPrivate)

  // VertexData
  static void Bind(const VertexDataInstance& data);
  static unsigned GetNVertices(const VertexDataInstance& data);
  static unsigned GetLayoutSize(const VertexDataInstance& data);
  static const DataLayout& GetDataLayout(const VertexDataInstance& data);
  static void AddListener(VertexDataInstance& data,
                          IDataChangeListener* listener);
  static void RemoveListener(VertexDataInstance& data,
                             IDataChangeListener* listener);

  // IndexData
  static void Bind(const IndexDataInstance& data);
  static unsigned GetNIndices(const IndexDataInstance& data);
  static void AddListener(IndexDataInstance& data,
                          IDataChangeListener* listener);
  static void RemoveListener(IndexDataInstance& data,
                             IDataChangeListener* listener);

  // InstancedDataBuffer
  static void Bind(const InstancedDataInstance& buffer);
  static unsigned GetNInstances(const InstancedDataInstance& buffer);
  static unsigned GetInstanceSize(const InstancedDataInstance& buffer);
  static const DataLayout& GetDataLayout(const InstancedDataInstance& data);
  static void AddListener(InstancedDataInstance& data,
                          IDataChangeListener* listener);
  static void RemoveListener(InstancedDataInstance& data,
                             IDataChangeListener* listener);

  // ShaderBinding
  static const VertexDataInstance& GetVertexData(const ShaderBinding& binding);
  static const InstancedDataInstance& GetInstancedData(
    const ShaderBinding& binding);
  static unsigned GetLocation(const ShaderBinding& binding);
  static unsigned GetNEntries(const ShaderBinding& binding);
  static unsigned GetDataType(const ShaderBinding& binding);
  static unsigned GetOffset(const ShaderBinding& binding);

  // Shader
  static const DataLayout& GetDataLayout(const ShaderInstance& shader);
  static unsigned GetAttributeLocation(const std::string& attributeName,
                                       const ShaderInstance& shader);
  static void UseShader(const ShaderInstance& shader);

  // Framebuffer
  static void UseFramebuffer(const FrameBufferInstance& frameBuffer);

  // Texture
  static unsigned GetTextureID(const Texture2D& texture);
  static void UseTexture(const Texture2D& texture, const TextureUnit& unit);

  // Target
  static const ShaderInstance& GetShader(const TargetInstance& target);
  static const FrameBufferInstance& GetFramebuffer(
    const TargetInstance& target);
};

// --------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------
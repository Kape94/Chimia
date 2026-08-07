#include "BufferPrivate.h"
#include "DataListeners.h"
#include "IndexData.h"
#include "ShaderBinding.h"
#include "VertexData.h"

// --------------------------------------------------------------------------------------

USING_RENDERLIB_NAMESPACE

// --------------------------------------------------------------------------------------

void
BufferPrivate::Bind(const VertexDataInstance& data)
{
  data->Bind();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetNVertices(const VertexDataInstance& data)
{
  return data->GetNVertices();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetLayoutSize(const VertexDataInstance& data)
{
  return data->GetLayoutSize();
}

// --------------------------------------------------------------------------------------

const DataLayout&
BufferPrivate::GetDataLayout(const VertexDataInstance& data)
{
  return data->GetDataLayout();
}

// --------------------------------------------------------------------------------------

void
BufferPrivate::AddListener(VertexDataInstance& data,
                           IDataChangeListener* listener)
{
  data->GetListeners().Add(listener);
}

// --------------------------------------------------------------------------------------

void
BufferPrivate::RemoveListener(VertexDataInstance& data,
                              IDataChangeListener* listener)
{
  data->GetListeners().Remove(listener);
}

// --------------------------------------------------------------------------------------

void
BufferPrivate::Bind(const IndexDataInstance& data)
{
  data->Bind();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetNIndices(const IndexDataInstance& data)
{
  return data->GetNIndices();
}

// --------------------------------------------------------------------------------------

void
BufferPrivate::AddListener(IndexDataInstance& data,
                           IDataChangeListener* listener)
{
  data->GetListeners().Add(listener);
}

// --------------------------------------------------------------------------------------

void
BufferPrivate::RemoveListener(IndexDataInstance& data,
                              IDataChangeListener* listener)
{
  data->GetListeners().Remove(listener);
}

// --------------------------------------------------------------------------------------

void
BufferPrivate::Bind(const InstancedDataInstance& buffer)
{
  buffer->Bind();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetNInstances(const InstancedDataInstance& buffer)
{
  return buffer->GetNInstances();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetInstanceSize(const InstancedDataInstance& buffer)
{
  return buffer->GetInstanceSize();
}

// --------------------------------------------------------------------------------------

const DataLayout&
BufferPrivate::GetDataLayout(const InstancedDataInstance& data)
{
  return data->GetDataLayout();
}

// --------------------------------------------------------------------------------------

const VertexDataInstance&
BufferPrivate::GetVertexData(const ShaderBinding& binding)
{
  return binding.GetVertexData();
}

// --------------------------------------------------------------------------------------

const InstancedDataInstance&
BufferPrivate::GetInstancedData(const ShaderBinding& binding)
{
  return binding.GetInstancedData();
}

// --------------------------------------------------------------------------------------

void
BufferPrivate::AddListener(InstancedDataInstance& data,
                           IDataChangeListener* listener)
{
  data->GetListeners().Add(listener);
}

// --------------------------------------------------------------------------------------

void
BufferPrivate::RemoveListener(InstancedDataInstance& data,
                              IDataChangeListener* listener)
{
  data->GetListeners().Remove(listener);
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetLocation(const ShaderBinding& binding)
{
  return binding.GetLocation();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetNEntries(const ShaderBinding& binding)
{
  return binding.GetNEntries();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetDataType(const ShaderBinding& binding)
{
  return binding.GetDataType();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetOffset(const ShaderBinding& binding)
{
  return binding.GetOffset();
}

// --------------------------------------------------------------------------------------

const DataLayout&
BufferPrivate::GetDataLayout(const ShaderInstance& shader)
{
  return shader->GetDataLayout();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetAttributeLocation(const std::string& attributeName,
                                    const ShaderInstance& shader)
{
  return shader->GetLocationOfAttribute(attributeName);
}

// --------------------------------------------------------------------------------------

void
BufferPrivate::UseShader(const ShaderInstance& shader)
{
  shader->Use();
}

// --------------------------------------------------------------------------------------

void
BufferPrivate::UseFramebuffer(const FrameBufferInstance& frameBuffer)
{
  frameBuffer->Use();
}

// --------------------------------------------------------------------------------------

unsigned
BufferPrivate::GetTextureID(const Texture2DInstance& texture)
{
  return texture->m_id;
}

// --------------------------------------------------------------------------------------

void
BufferPrivate::UseTexture(const Texture2DInstance& texture,
                          const TextureUnit& unit)
{
  texture->Use(unit);
}

// --------------------------------------------------------------------------------------

const ShaderInstance&
BufferPrivate::GetShader(const TargetInstance& target)
{
  return target->m_shader;
}

// --------------------------------------------------------------------------------------

const FrameBufferInstance&
BufferPrivate::GetFramebuffer(const TargetInstance& target)
{
  return target->m_framebuffer;
}

// --------------------------------------------------------------------------------------
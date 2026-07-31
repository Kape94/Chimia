#pragma once

//---------------------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "InstancedData.h"
#include "RenderingNamespaceDefs.h"
#include "Shader.h"
#include "VertexData.h"
#include <initializer_list>

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

class ShaderBindings;

//---------------------------------------------------------------------------------------
// ShaderBinding
//---------------------------------------------------------------------------------------

class ShaderBinding
{
public:
  NON_DEFAULT_CONSTRUCTIBLE(ShaderBinding)

  ShaderBinding(const ShaderBinding& other);
  ShaderBinding& operator=(const ShaderBinding& other);

  static ShaderBindings Connect(const VertexDataInstance& data,
                                const std::string& vertexAttribute,
                                const Shader& shader,
                                const std::string& shaderAttribute);

  static ShaderBindings Connect(const InstancedDataInstance& data,
                                const std::string& vertexAttribute,
                                const Shader& shader,
                                const std::string& shaderAttribute);

private:
  friend class BufferPrivate;

  ShaderBinding(const VertexDataInstance& vertexData,
                const InstancedDataInstance& instancedData,
                const unsigned location,
                const unsigned nEntries,
                const unsigned dataType,
                const unsigned offset);

  const VertexDataInstance& GetVertexData() const;
  const InstancedDataInstance& GetInstancedData() const;
  unsigned GetLocation() const;
  unsigned GetNEntries() const;
  unsigned GetDataType() const;
  unsigned GetOffset() const;

  VertexDataInstance m_referenceVertexData = nullptr;
  InstancedDataInstance m_referenceInstancedData = nullptr;

  unsigned m_location = 0;
  unsigned m_nEntries = 0;
  unsigned m_dataType = 0;
  unsigned m_dataSizeInBytes = 0;
  unsigned m_offset = 0;
};

//---------------------------------------------------------------------------------------
// ShaderBindings
//---------------------------------------------------------------------------------------

class ShaderBindings
{
public:
  ShaderBindings() = default;
  ShaderBindings(const std::initializer_list<ShaderBindings>& bindingsList);
  ShaderBindings(const std::initializer_list<ShaderBinding>& bindings);

  void Insert(const ShaderBinding& binding);
  void Insert(const ShaderBindings& bindings);
  void Clear();

  std::vector<ShaderBinding>::const_iterator begin() const;
  std::vector<ShaderBinding>::const_iterator end() const;

private:
  std::vector<ShaderBinding> m_bindings;
};

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------
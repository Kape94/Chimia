#pragma once

//---------------------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "InstancedData.h"
#include "RenderingNamespaceDefs.h"
#include "VertexData.h"

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

class ShaderBinding
{
public:
  NON_DEFAULT_CONSTRUCTIBLE(ShaderBinding)

  ShaderBinding(const ShaderBinding& other);
  ShaderBinding& operator=(const ShaderBinding& other);

  static ShaderBinding Float(const VertexData& data,
                             const unsigned location,
                             const unsigned nEntries,
                             const unsigned offset);

  static ShaderBinding Double(const VertexData& data,
                              const unsigned location,
                              const unsigned nEntries,
                              const unsigned offset);

  static ShaderBinding Int(const VertexData& data,
                           const unsigned location,
                           const unsigned nEntries,
                           const unsigned offset);

  static ShaderBinding UnsignedInt(const VertexData& data,
                                   const unsigned location,
                                   const unsigned nEntries,
                                   const unsigned offset);

  static ShaderBinding Short(const VertexData& data,
                             const unsigned location,
                             const unsigned nEntries,
                             const unsigned offset);

  static ShaderBinding UnsignedShort(const VertexData& data,
                                     const unsigned location,
                                     const unsigned nEntries,
                                     const unsigned offset);

  static ShaderBinding Byte(const VertexData& data,
                            const unsigned location,
                            const unsigned nEntries,
                            const unsigned offset);

  static ShaderBinding UnsignedByte(const VertexData& data,
                                    const unsigned location,
                                    const unsigned nEntries,
                                    const unsigned offset);

  static ShaderBinding Float(const InstancedData& data,
                             const unsigned location,
                             const unsigned nEntries,
                             const unsigned offset);

  static ShaderBinding Double(const InstancedData& data,
                              const unsigned location,
                              const unsigned nEntries,
                              const unsigned offset);

  static ShaderBinding Int(const InstancedData& data,
                           const unsigned location,
                           const unsigned nEntries,
                           const unsigned offset);

  static ShaderBinding UnsignedInt(const InstancedData& data,
                                   const unsigned location,
                                   const unsigned nEntries,
                                   const unsigned offset);

  static ShaderBinding Short(const InstancedData& data,
                             const unsigned location,
                             const unsigned nEntries,
                             const unsigned offset);

  static ShaderBinding UnsignedShort(const InstancedData& data,
                                     const unsigned location,
                                     const unsigned nEntries,
                                     const unsigned offset);

  static ShaderBinding Byte(const InstancedData& data,
                            const unsigned location,
                            const unsigned nEntries,
                            const unsigned offset);

  static ShaderBinding UnsignedByte(const InstancedData& data,
                                    const unsigned location,
                                    const unsigned nEntries,
                                    const unsigned offset);

private:
  friend class BufferPrivate;

  ShaderBinding(const VertexData* vertexData,
                const InstancedData* instancedData,
                const unsigned location,
                const unsigned nEntries,
                const unsigned dataType,
                const unsigned offset);

  const VertexData* GetVertexData() const;
  const InstancedData* GetInstancedData() const;
  unsigned GetLocation() const;
  unsigned GetNEntries() const;
  unsigned GetDataType() const;
  unsigned GetOffset() const;

  const VertexData* m_referenceVertexData = nullptr;
  const InstancedData* m_referenceInstancedData = nullptr;

  unsigned m_location = 0;
  unsigned m_nEntries = 0;
  unsigned m_dataType = 0;
  unsigned m_dataSizeInBytes = 0;
  unsigned m_offset = 0;
};

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------
#pragma once

#include "Core/ClassDefs.h"
#include "GenericRenderAction.h"
#include "RenderingNamespaceDefs.h"

#include "ShaderAttribute.h"

#include "Core/Types.h"

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

class IndexedRenderAction
{
public:
  DEFAULT_CONSTUCTIBLE(IndexedRenderAction)

  NON_COPYABLE(IndexedRenderAction)

  IndexedRenderAction(IndexedRenderAction&& other) noexcept;
  IndexedRenderAction& operator=(IndexedRenderAction&& other) noexcept;

  ~IndexedRenderAction();

  void Create(const VertexDataInstance& reusableVertexBuffer,
              const IndexDataInstance& reusableIndexBuffer,
              const ShaderAttributes& shaderAttributes);

  void Clear();

  void Render() const;

private:
  GenericRenderAction m_action;
};

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------
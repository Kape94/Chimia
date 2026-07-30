#pragma once

#include "Core/ClassDefs.h"
#include "IndexData.h"
#include "InstancedData.h"
#include "Rendering/IDataChangeListener.h"
#include "RenderingNamespaceDefs.h"
#include "ShaderBinding.h"
#include "VertexData.h"

//---------------------------------------------------------------------------------------

BEGIN_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------

class RenderAction : public IDataChangeListener
{
public:
  DEFAULT_CONSTUCTIBLE(RenderAction)
  NON_COPYABLE(RenderAction)

  RenderAction(RenderAction&& other) noexcept;
  RenderAction& operator=(RenderAction&& other) noexcept;

  ~RenderAction();

  void Create(const ShaderBindings& bindings);

  void Create(const ShaderBindings& bindings,
              const IndexDataInstance& indexData);

  void Clear();

  void Render() const;

private:
  void SetupVAO();

  void ClearRenderingData();

  void CollectDatasFromBindings(const ShaderBindings& bindings);

  void RegisterAsListener();
  void UnregisterAsListener();

  void RenderInstanced() const;

  void RenderSingle() const;

  unsigned PickReferenceVertexCount() const;
  unsigned PickReferenceInstanceCount() const;

  void DataChanged() override;

  unsigned m_VAO = 0;

  std::vector<VertexDataInstance> m_referenceVertexDatas;
  IndexDataInstance m_referenceIndexBuffer = nullptr;
  std::vector<InstancedDataInstance> m_referenceInstancedDatas;

  ShaderBindings m_bindings;
};

//---------------------------------------------------------------------------------------

END_RENDERLIB_NAMESPACE

//---------------------------------------------------------------------------------------
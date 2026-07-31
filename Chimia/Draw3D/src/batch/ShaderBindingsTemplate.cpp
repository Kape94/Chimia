#include "ShaderBindingsTemplate.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

//-----------------------------------------------------------------------------

ShaderBindingsTemplate::ShaderBindingsTemplate(
  const std::initializer_list<Item>& items,
  const Rendering::Shader& shader)
  : m_templates(items)
  , m_shader(&shader)
{
}

//-----------------------------------------------------------------------------

std::vector<Chimia::Rendering::RenderAction::Binding>
ShaderBindingsTemplate::GenerateFor(
  const Rendering::VertexDataInstance& vertexData) const
{
  std::vector<Rendering::RenderAction::Binding> bindings;

  for (const auto& item : m_templates) {
    bindings.push_back({ vertexData, item.sourceName, item.destinationName });
  }

  return bindings;
}

//-----------------------------------------------------------------------------

std::vector<Chimia::Rendering::RenderAction::Binding>
ShaderBindingsTemplate::GenerateFor(
  const Rendering::InstancedDataInstance& instancedData) const
{
  std::vector<Rendering::RenderAction::Binding> bindings;

  for (const auto& item : m_templates) {
    bindings.push_back(
      { instancedData, item.sourceName, item.destinationName });
  }

  return bindings;
}

// ----------------------------------------------------------------------------

const Chimia::Rendering::Shader&
ShaderBindingsTemplate::GetShader() const
{
  return *m_shader;
}

// ----------------------------------------------------------------------------
#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include "Rendering/InstancedData.h"
#include "Rendering/RenderAction.h"
#include "Rendering/Shader.h"
#include "Rendering/VertexData.h"

#include <string>
#include <vector>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

//---------------------------------------------------------------------------------------

class ShaderBindingsTemplate
{
private:
  struct Item
  {
    std::string sourceName;
    std::string destinationName;
  };

public:
  ShaderBindingsTemplate() = default;
  ShaderBindingsTemplate(const std::initializer_list<Item>& items,
                         const Rendering::Shader& shader);

  std::vector<Rendering::RenderAction::Binding> GenerateFor(
    const Rendering::VertexDataInstance& vertexData) const;
  std::vector<Rendering::RenderAction::Binding> GenerateFor(
    const Rendering::InstancedDataInstance& instancedData) const;

  const Rendering::Shader& GetShader() const;

private:
  std::vector<Item> m_templates;
  const Rendering::Shader* m_shader = nullptr;
};

//---------------------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

//---------------------------------------------------------------------------------------

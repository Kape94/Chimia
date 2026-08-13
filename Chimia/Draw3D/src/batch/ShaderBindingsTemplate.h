#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include "Rendering/InstancedData.h"
#include "Rendering/RenderAction.h"
#include "Rendering/VertexData.h"

#include <string>
#include <vector>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

//---------------------------------------------------------------------------------------

class ShaderBindingsTemplate
{
public:
  struct Item
  {
    std::string sourceName;
    std::string destinationName;
  };

  ShaderBindingsTemplate() = default;

  ShaderBindingsTemplate(const std::vector<Item>& items);

  ShaderBindingsTemplate(const std::initializer_list<Item>& items);

  std::vector<Rendering::RenderAction::Binding> GenerateFor(
    const Rendering::VertexDataInstance& vertexData) const;
  std::vector<Rendering::RenderAction::Binding> GenerateFor(
    const Rendering::InstancedDataInstance& instancedData) const;

private:
  std::vector<Item> m_templates;
};

//---------------------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

//---------------------------------------------------------------------------------------

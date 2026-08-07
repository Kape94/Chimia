#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include "Rendering/InstancedData.h"
#include "Rendering/RenderAction.h"
#include "Rendering/Target.h"
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
                         const Rendering::TargetInstance& target);

  std::vector<Rendering::RenderAction::Binding> GenerateFor(
    const Rendering::VertexDataInstance& vertexData) const;
  std::vector<Rendering::RenderAction::Binding> GenerateFor(
    const Rendering::InstancedDataInstance& instancedData) const;

  const Rendering::TargetInstance& GetTarget() const;

private:
  std::vector<Item> m_templates;
  Rendering::TargetInstance m_target = nullptr;
};

//---------------------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

//---------------------------------------------------------------------------------------

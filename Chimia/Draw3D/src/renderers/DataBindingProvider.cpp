#include "DataBindingProvider.h"
#include "RenderersUtils.h"
#include "Rendering/DataLayout.h"

#include "DataNames.h"
#include "ShaderBindingsTemplate.h"

#include <vector>

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
// Helpers
// ----------------------------------------------------------------------------

namespace {

std::vector<Chimia::Rendering::DataLayout::DataSpec>
GetVertexLayoutSpecs(const eVertexLayout vertexLayout)
{
  std::vector<Chimia::Rendering::DataLayout::DataSpec> vertexSpecs;

  vertexSpecs.push_back({ DataNames::BufferData::VERTEX_POS,
                          Chimia::Rendering::eDataType::VECTOR_3_FLOAT });
  if (RenderersUtils::HasColor(vertexLayout)) {
    vertexSpecs.push_back({ DataNames::BufferData::VERTEX_COLOR,
                            Chimia::Rendering::eDataType::VECTOR_4_FLOAT });
  }
  if (RenderersUtils::HasNormal(vertexLayout)) {
    vertexSpecs.push_back({ DataNames::BufferData::VERTEX_NORMAL,
                            Chimia::Rendering::eDataType::VECTOR_3_FLOAT });
  }
  if (RenderersUtils::HasTexCoord(vertexLayout)) {
    vertexSpecs.push_back({ DataNames::BufferData::VERTEX_TEX_COORD,
                            Chimia::Rendering::eDataType::VECTOR_2_FLOAT });
  }

  return vertexSpecs;
}

// ----------------------------------------------------------------------------

std::vector<ShaderBindingsTemplate::Item>
GetVertexBindings(const eVertexLayout vertexLayout)
{
  std::vector<ShaderBindingsTemplate::Item> vertexBindings;

  vertexBindings.push_back(
    { DataNames::BufferData::VERTEX_POS, DataNames::ShaderInputs::VERTEX_POS });
  if (RenderersUtils::HasColor(vertexLayout)) {
    vertexBindings.push_back({ DataNames::BufferData::VERTEX_COLOR,
                               DataNames::ShaderInputs::VERTEX_COLOR });
  }
  if (RenderersUtils::HasNormal(vertexLayout)) {
    vertexBindings.push_back({ DataNames::BufferData::VERTEX_NORMAL,
                               DataNames::ShaderInputs::VERTEX_NORMAL });
  }
  if (RenderersUtils::HasTexCoord(vertexLayout)) {
    vertexBindings.push_back({ DataNames::BufferData::VERTEX_TEX_COORD,
                               DataNames::ShaderInputs::VERTEX_TEX_COORD });
  }

  return vertexBindings;
}

// ----------------------------------------------------------------------------

std::vector<ShaderBindingsTemplate::Item>
GetTargetVertexBindings(const eVertexLayout vertexLayout)
{
  std::vector<ShaderBindingsTemplate::Item> targetVertexBindings;

  targetVertexBindings.push_back(
    { DataNames::BufferData::VERTEX_POS,
      DataNames::ShaderInputs::VERTEX_TARGET_POS });
  if (RenderersUtils::HasColor(vertexLayout)) {
    targetVertexBindings.push_back(
      { DataNames::BufferData::VERTEX_COLOR,
        DataNames::ShaderInputs::VERTEX_TARGET_COLOR });
  }
  if (RenderersUtils::HasNormal(vertexLayout)) {
    targetVertexBindings.push_back(
      { DataNames::BufferData::VERTEX_NORMAL,
        DataNames::ShaderInputs::VERTEX_TARGET_NORMAL });
  }
  if (RenderersUtils::HasTexCoord(vertexLayout)) {
    targetVertexBindings.push_back(
      { DataNames::BufferData::VERTEX_TEX_COORD,
        DataNames::ShaderInputs::VERTEX_TARGET_TEX_COORD });
  }

  return targetVertexBindings;
}

}

// ----------------------------------------------------------------------------
// DataBindingProvider
// ----------------------------------------------------------------------------

DataBindingProvider::DataBindingProvider(
  const eVertexLayout vertexLayout,
  const Rendering::TargetInstance& target)
  : m_renderingTarget(target)
  , m_vertexLayout(GetVertexLayoutSpecs(vertexLayout))
  , m_instancedLayout({ { DataNames::BufferData::INSTANCE_TRANSFORM,
                          Rendering::eDataType::MATRIX_FLOAT_4X4 } })
  , m_instancedTransitionLayout(
      { { DataNames::BufferData::INSTANCE_TRANSFORM,
          Rendering::eDataType::MATRIX_FLOAT_4X4 },
        { DataNames::BufferData::INSTANCE_INTERPOLATION,
          Rendering::eDataType::FLOAT } })
  , m_vertexTemplate(GetVertexBindings(vertexLayout))
  , m_targetVertexTemplate(GetTargetVertexBindings(vertexLayout))
  , m_instancedTemplate({ { DataNames::BufferData::INSTANCE_TRANSFORM,
                            DataNames::ShaderInputs::INSTANCE_TRANSFORM } })
  , m_instancedTransitionTemplate(
      { { DataNames::BufferData::INSTANCE_TRANSFORM,
          DataNames::ShaderInputs::INSTANCE_TRANSFORM },
        { DataNames::BufferData::INSTANCE_INTERPOLATION,
          DataNames::ShaderInputs::INSTANCE_TRANSITION_INTERPOLATION } })
{
}

// ----------------------------------------------------------------------------

const Chimia::Rendering::DataLayout&
DataBindingProvider::GetVertexLayout() const
{
  return m_vertexLayout;
}

// ----------------------------------------------------------------------------

const Chimia::Rendering::DataLayout&
DataBindingProvider::GetInstancedLayout() const
{
  return m_instancedLayout;
}

// ----------------------------------------------------------------------------

const Chimia::Rendering::DataLayout&
DataBindingProvider::GetInstancedTransitionLayout() const
{
  return m_instancedTransitionLayout;
}

// ----------------------------------------------------------------------------

const ShaderBindingsTemplate&
DataBindingProvider::GetVertexTemplate() const
{
  return m_vertexTemplate;
}

// ----------------------------------------------------------------------------

const ShaderBindingsTemplate&
DataBindingProvider::GetTargetVertexTemplate() const
{
  return m_targetVertexTemplate;
}

// ----------------------------------------------------------------------------

const ShaderBindingsTemplate&
DataBindingProvider::GetInstancedTemplate() const
{
  return m_instancedTemplate;
}

// ----------------------------------------------------------------------------

const ShaderBindingsTemplate&
DataBindingProvider::GetInstancedTransitionTemplate() const
{
  return m_instancedTransitionTemplate;
}

// ----------------------------------------------------------------------------

const Chimia::Rendering::TargetInstance&
DataBindingProvider::GetRenderingTarget() const
{
  return m_renderingTarget;
}

// ----------------------------------------------------------------------------
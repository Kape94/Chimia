#include "DataBindingProvider.h"
#include "RenderersUtils.h"
#include "Rendering/DataLayout.h"

#include "DataNames.h"
#include "ShaderBindingsTemplate.h"

#include <vector>

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

DataBindingProvider::DataBindingProvider(
  const eVertexLayout vertexLayout,
  const Rendering::TargetInstance& target)
  : m_renderingTarget(target)
{
  const bool hasColor = RenderersUtils::HasColor(vertexLayout);
  const bool hasNormal = RenderersUtils::HasNormal(vertexLayout);
  const bool hasTexCoord = RenderersUtils::HasTexCoord(vertexLayout);

  std::vector<Rendering::DataLayout::DataSpec> vertexSpecs;
  vertexSpecs.push_back({ DataNames::BufferData::VERTEX_POS,
                          Chimia::Rendering::eDataType::VECTOR_3_FLOAT });
  if (hasColor) {
    vertexSpecs.push_back({ DataNames::BufferData::VERTEX_COLOR,
                            Chimia::Rendering::eDataType::VECTOR_4_FLOAT });
  }
  if (hasNormal) {
    vertexSpecs.push_back({ DataNames::BufferData::VERTEX_NORMAL,
                            Chimia::Rendering::eDataType::VECTOR_3_FLOAT });
  }
  if (hasTexCoord) {
    vertexSpecs.push_back({ DataNames::BufferData::VERTEX_TEX_COORD,
                            Chimia::Rendering::eDataType::VECTOR_2_FLOAT });
  }

  m_vertexLayout = Rendering::DataLayout(vertexSpecs);

  m_instancedLayout =
    Rendering::DataLayout({ { DataNames::BufferData::INSTANCE_TRANSFORM,
                              Rendering::eDataType::MATRIX_FLOAT_4X4 } });

  m_instancedTransitionLayout =
    Rendering::DataLayout({ { DataNames::BufferData::INSTANCE_TRANSFORM,
                              Rendering::eDataType::MATRIX_FLOAT_4X4 },
                            { DataNames::BufferData::INSTANCE_INTERPOLATION,
                              Rendering::eDataType::FLOAT } });

  std::vector<ShaderBindingsTemplate::Item> vertexBindings;
  vertexBindings.push_back(
    { DataNames::BufferData::VERTEX_POS, DataNames::ShaderInputs::VERTEX_POS });
  if (hasColor) {
    vertexBindings.push_back({ DataNames::BufferData::VERTEX_COLOR,
                               DataNames::ShaderInputs::VERTEX_COLOR });
  }
  if (hasNormal) {
    vertexBindings.push_back({ DataNames::BufferData::VERTEX_NORMAL,
                               DataNames::ShaderInputs::VERTEX_NORMAL });
  }
  if (hasTexCoord) {
    vertexBindings.push_back({ DataNames::BufferData::VERTEX_TEX_COORD,
                               DataNames::ShaderInputs::VERTEX_TEX_COORD });
  }
  m_vertexTemplate = ShaderBindingsTemplate(vertexBindings);

  std::vector<ShaderBindingsTemplate::Item> targetVertexBindings;
  targetVertexBindings.push_back(
    { DataNames::BufferData::VERTEX_POS,
      DataNames::ShaderInputs::VERTEX_TARGET_POS });
  if (hasColor) {
    targetVertexBindings.push_back(
      { DataNames::BufferData::VERTEX_COLOR,
        DataNames::ShaderInputs::VERTEX_TARGET_COLOR });
  }
  if (hasNormal) {
    targetVertexBindings.push_back(
      { DataNames::BufferData::VERTEX_NORMAL,
        DataNames::ShaderInputs::VERTEX_TARGET_NORMAL });
  }
  if (hasTexCoord) {
    targetVertexBindings.push_back(
      { DataNames::BufferData::VERTEX_TEX_COORD,
        DataNames::ShaderInputs::VERTEX_TARGET_TEX_COORD });
  }
  m_targetVertexTemplate = ShaderBindingsTemplate(targetVertexBindings);

  m_instancedTemplate =
    ShaderBindingsTemplate({ { DataNames::BufferData::INSTANCE_TRANSFORM,
                               DataNames::ShaderInputs::INSTANCE_TRANSFORM } });

  m_instancedTransitionTemplate = ShaderBindingsTemplate(
    { { DataNames::BufferData::INSTANCE_TRANSFORM,
        DataNames::ShaderInputs::INSTANCE_TRANSFORM },
      { DataNames::BufferData::INSTANCE_INTERPOLATION,
        DataNames::ShaderInputs::INSTANCE_TRANSITION_INTERPOLATION } });
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
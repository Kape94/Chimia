#include "Pipeline.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

Pipeline::Pipeline(
  const std::function<void(void)> activateFunction,
  const std::function<void(Chimia::Rendering::Shader&)> configureShader)
  : m_activateFunction(activateFunction)
  , m_configureFunction(configureShader)
{
}

// ----------------------------------------------------------------------------

void
Pipeline::Use() const
{
  m_activateFunction();
}

// ----------------------------------------------------------------------------

void
Pipeline::ConfigureShader(Chimia::Rendering::Shader& shader) const
{
  m_configureFunction(shader);
}

// ----------------------------------------------------------------------------
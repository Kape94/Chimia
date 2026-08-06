#pragma once

// ----------------------------------------------------------------------------

#include "Core/ClassDefs.h"
#include "Draw3DNamespaceDefs.h"
#include "Rendering/Shader.h"
#include <functional>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class Pipeline
{
public:
  NON_COPYABLE_NON_MOVABLE(Pipeline)

  Pipeline(const std::function<void(void)> activateFunction,
           const std::function<void(Chimia::Rendering::ShaderInstance&)>
             configureShader);

  void Use() const;
  void ConfigureShader(Chimia::Rendering::ShaderInstance& shader) const;

private:
  std::function<void(void)> m_activateFunction;
  std::function<void(Chimia::Rendering::ShaderInstance&)> m_configureFunction;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
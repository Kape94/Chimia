#pragma once

#include "Draw3DNamespaceDefs.h"

#include "Rendering/Buffer.h"
#include "Rendering/ShaderAttribute.h"

#include <vector>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class StaticTriangles
{
public:
  StaticTriangles() = default;

  void Create(const std::vector<float>& vertexData,
              const Rendering::ShaderAttributes& shaderAttributes);

  void Render() const;

private:
  Rendering::Buffer m_gpuBuffer;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
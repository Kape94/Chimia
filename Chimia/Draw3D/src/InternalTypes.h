#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include <glm/vec3.hpp>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

class LocalModelInstanceID
{
private:
  friend class Draw3DPrivate;

  LocalModelInstanceID() = delete;
  LocalModelInstanceID(unsigned modelID, unsigned instanceID)
    : m_modelID(modelID)
    , m_instanceID(instanceID)
  {
  }

  unsigned m_modelID = 0;
  unsigned m_instanceID = 0;
};

struct Material
{
  glm::vec3 ambient{ 0.0f, 0.0f, 0.0f };
  glm::vec3 diffuse{ 0.0f, 0.0f, 0.0f };
  glm::vec3 specular{ 0.0f, 0.0f, 0.0f };
  float shininess;
};

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
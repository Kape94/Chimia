#pragma once

#include "AppToolkitDefs.h"
#include "InputDefs.h"

#include <glm/vec2.hpp>

_BEGIN_APP_TOOLKIT_NAMESPACE

class IAppUtils
{
public:
  virtual ~IAppUtils() = default;

  virtual bool IsPressed(const Key key) = 0;
  virtual bool IsPressed(const MouseButton mouseButton) = 0;

  virtual bool IsReleased(const Key key) = 0;
  virtual bool IsReleased(const MouseButton mouseButton) = 0;

  virtual bool IsHold(const Key key) = 0;
  virtual bool IsHold(const MouseButton mouseButton) = 0;

  virtual void Exit() = 0;

  virtual glm::vec2 GetMousePos() const = 0;
  virtual void SetMousePos(const glm::vec2& pos) = 0;
  virtual glm::vec2 GetMouseMotion() const = 0;
};

_END_APP_TOOLKIT_NAMESPACE

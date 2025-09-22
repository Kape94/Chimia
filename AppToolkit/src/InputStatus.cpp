#include "InputStatus.h"

//-----------------------------------------------------------------------------

_USING_APP_TOOLKIT_NAMESPACE

//-----------------------------------------------------------------------------

bool
InputStatus::IsPressed(const Key key)
{
  const bool previousIsPressed = previousKeyState[key];
  const bool currentIsPressed = currentKeyState[key];

  return !previousIsPressed && currentIsPressed;
}

//-----------------------------------------------------------------------------

bool
InputStatus::IsPressed(const MouseButton mouseButton)
{
  const bool previousIsPressed = previousMouseButtonState[mouseButton];
  const bool currentIsPressed = currentMouseButtonState[mouseButton];

  return !previousIsPressed && currentIsPressed;
}

//-----------------------------------------------------------------------------

bool
InputStatus::IsReleased(const Key key)
{
  const bool previousIsPressed = previousKeyState[key];
  const bool currentIsPressed = currentKeyState[key];

  return previousIsPressed && !currentIsPressed;
}

//-----------------------------------------------------------------------------

bool
InputStatus::IsReleased(const MouseButton mouseButton)
{
  const bool previousIsPressed = previousMouseButtonState[mouseButton];
  const bool currentIsPressed = currentMouseButtonState[mouseButton];

  return previousIsPressed && !currentIsPressed;
}

//-----------------------------------------------------------------------------

bool
InputStatus::IsHold(const Key key)
{
  const bool previousIsPressed = previousKeyState[key];
  const bool currentIsPressed = currentKeyState[key];

  return previousIsPressed && currentIsPressed;
}

//-----------------------------------------------------------------------------

bool
InputStatus::IsHold(const MouseButton mouseButton)
{
  const bool previousIsPressed = previousMouseButtonState[mouseButton];
  const bool currentIsPressed = currentMouseButtonState[mouseButton];

  return previousIsPressed && currentIsPressed;
}

//-----------------------------------------------------------------------------

void
InputStatus::Poll(GLFWwindow* window)
{
  auto IsPressed = [window](int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
  };

  previousKeyState = currentKeyState;

  currentKeyState[Key::A] = IsPressed(GLFW_KEY_A);
  currentKeyState[Key::B] = IsPressed(GLFW_KEY_B);
  currentKeyState[Key::C] = IsPressed(GLFW_KEY_C);
  currentKeyState[Key::D] = IsPressed(GLFW_KEY_D);
  currentKeyState[Key::E] = IsPressed(GLFW_KEY_E);
  currentKeyState[Key::F] = IsPressed(GLFW_KEY_F);
  currentKeyState[Key::G] = IsPressed(GLFW_KEY_G);
  currentKeyState[Key::H] = IsPressed(GLFW_KEY_H);
  currentKeyState[Key::I] = IsPressed(GLFW_KEY_I);
  currentKeyState[Key::J] = IsPressed(GLFW_KEY_J);
  currentKeyState[Key::K] = IsPressed(GLFW_KEY_K);
  currentKeyState[Key::L] = IsPressed(GLFW_KEY_L);
  currentKeyState[Key::M] = IsPressed(GLFW_KEY_M);
  currentKeyState[Key::N] = IsPressed(GLFW_KEY_N);
  currentKeyState[Key::O] = IsPressed(GLFW_KEY_O);
  currentKeyState[Key::P] = IsPressed(GLFW_KEY_P);
  currentKeyState[Key::Q] = IsPressed(GLFW_KEY_Q);
  currentKeyState[Key::R] = IsPressed(GLFW_KEY_R);
  currentKeyState[Key::S] = IsPressed(GLFW_KEY_S);
  currentKeyState[Key::T] = IsPressed(GLFW_KEY_T);
  currentKeyState[Key::U] = IsPressed(GLFW_KEY_U);
  currentKeyState[Key::V] = IsPressed(GLFW_KEY_V);
  currentKeyState[Key::X] = IsPressed(GLFW_KEY_X);
  currentKeyState[Key::Y] = IsPressed(GLFW_KEY_Y);
  currentKeyState[Key::Z] = IsPressed(GLFW_KEY_Z);
  currentKeyState[Key::W] = IsPressed(GLFW_KEY_W);
  currentKeyState[Key::NUM_0] = IsPressed(GLFW_KEY_0);
  currentKeyState[Key::NUM_1] = IsPressed(GLFW_KEY_1);
  currentKeyState[Key::NUM_2] = IsPressed(GLFW_KEY_2);
  currentKeyState[Key::NUM_3] = IsPressed(GLFW_KEY_3);
  currentKeyState[Key::NUM_4] = IsPressed(GLFW_KEY_4);
  currentKeyState[Key::NUM_5] = IsPressed(GLFW_KEY_5);
  currentKeyState[Key::NUM_6] = IsPressed(GLFW_KEY_6);
  currentKeyState[Key::NUM_7] = IsPressed(GLFW_KEY_7);
  currentKeyState[Key::NUM_8] = IsPressed(GLFW_KEY_8);
  currentKeyState[Key::NUM_9] = IsPressed(GLFW_KEY_9);
  currentKeyState[Key::CTRL] = IsPressed(GLFW_KEY_LEFT_CONTROL);
  currentKeyState[Key::SHIFT] = IsPressed(GLFW_KEY_LEFT_SHIFT);
  currentKeyState[Key::ALT] = IsPressed(GLFW_KEY_LEFT_ALT);
  currentKeyState[Key::ESC] = IsPressed(GLFW_KEY_ESCAPE);

  previousMouseButtonState = currentMouseButtonState;

  auto IsMouseButtonPressed = [window](int mouseButton) {
    return glfwGetMouseButton(window, mouseButton) == GLFW_PRESS;
  };

  currentMouseButtonState[MouseButton::LEFT] =
    IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);
  currentMouseButtonState[MouseButton::RIGHT] =
    IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT);
  currentMouseButtonState[MouseButton::MIDDLE] =
    IsMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE);
}

//-----------------------------------------------------------------------------

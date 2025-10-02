#include "AppImpl.h"

#include "OpenGLHelper.h"

#include "InputDefs.h"

//-----------------------------------------------------------------------------

USING_CHIMIA_APP_NAMESPACE

//-----------------------------------------------------------------------------

void
AppImpl::Run()
{
  Initialize();

  while (ShouldKeepRunning()) {
    ProcessFrame();
  }

  Cleanup();
}

//-----------------------------------------------------------------------------

void
AppImpl::Initialize()
{
  InitializeContext();
  InitializeApp();
}

//-----------------------------------------------------------------------------

void
AppImpl::InitializeContext()
{
  windowContext.Init(windowProperties);
  OpenGLHelper::Init(windowProperties.width, windowProperties.height);

  uiContext.Init(windowContext.GetWindow());
}

//-----------------------------------------------------------------------------

void
AppImpl::InitializeApp()
{
  if (appInitializer != nullptr) {
    appInitializer->Initialize();
  }
}

//-----------------------------------------------------------------------------

bool
AppImpl::ShouldKeepRunning() const
{
  return isRunning && !windowContext.ShouldClose();
}

//-----------------------------------------------------------------------------

void
AppImpl::ProcessFrame()
{
  BeginFrame();

  Update();
  Render();
  PresentUI();

  EndFrame();
}

//-----------------------------------------------------------------------------

void
AppImpl::BeginFrame()
{
  OpenGLHelper::Clear(0.0f, 0.0f, 0.0f);
}

//-----------------------------------------------------------------------------

void
AppImpl::PresentUI()
{
  uiContext.PresentUI();
}

//-----------------------------------------------------------------------------

void
AppImpl::Update()
{
  if (appUpdater != nullptr) {
    appUpdater->Update(*this);
  }
}

//-----------------------------------------------------------------------------

void
AppImpl::Render()
{
  if (appRenderer != nullptr) {
    appRenderer->Render();
  }
}

//-----------------------------------------------------------------------------

void
AppImpl::EndFrame()
{
  OpenGLHelper::Flush();
  windowContext.Swap();
  windowContext.PollEvents();
  inputStatus.Poll(windowContext.GetWindow());
}

//-----------------------------------------------------------------------------

void
AppImpl::Cleanup()
{
  if (appCleanup != nullptr) {
    appCleanup->Cleanup();
  }

  uiContext.Cleanup();
  windowContext.Cleanup();
}

//-----------------------------------------------------------------------------

void
AppImpl::DefineWindowProperties(const WindowProperties& _windowProperties)
{
  this->windowProperties = _windowProperties;
}

//-----------------------------------------------------------------------------

void
AppImpl::SetUiPresenter(IuiPresenter* uiPresenter)
{
  uiContext.SetPresenter(uiPresenter);
}

//-----------------------------------------------------------------------------

void
AppImpl::SetAppInitializer(IAppInitializer* _appInitializer)
{
  appInitializer = _appInitializer;
}

//-----------------------------------------------------------------------------

void
AppImpl::SetAppRenderer(IAppRenderer* _appRenderer)
{
  appRenderer = _appRenderer;
}

//-----------------------------------------------------------------------------

void
AppImpl::SetAppUpdater(IAppUpdater* _appUpdater)
{
  appUpdater = _appUpdater;
}

//-----------------------------------------------------------------------------

void
AppImpl::SetAppCleanup(IAppCleanup* _appCleanup)
{
  appCleanup = _appCleanup;
}

//-----------------------------------------------------------------------------
// IAppUtils
//-----------------------------------------------------------------------------

bool
AppImpl::IsPressed(const Key key)
{
  return inputStatus.IsPressed(key);
}

//-----------------------------------------------------------------------------

bool
AppImpl::IsPressed(const MouseButton mouseButton)
{
  return inputStatus.IsPressed(mouseButton);
}

//-----------------------------------------------------------------------------

bool
AppImpl::IsReleased(const Key key)
{
  return inputStatus.IsReleased(key);
}

//-----------------------------------------------------------------------------

bool
AppImpl::IsReleased(const MouseButton mouseButton)
{
  return inputStatus.IsReleased(mouseButton);
}

//-----------------------------------------------------------------------------

bool
AppImpl::IsHold(const Key key)
{
  return inputStatus.IsHold(key);
}

//-----------------------------------------------------------------------------

bool
AppImpl::IsHold(const MouseButton mouseButton)
{
  return inputStatus.IsHold(mouseButton);
}

//-----------------------------------------------------------------------------

void
AppImpl::Exit()
{
  isRunning = false;
}

//-----------------------------------------------------------------------------

glm::vec2
AppImpl::GetMousePos() const
{
  return windowContext.GetCursorPos();
}

//-----------------------------------------------------------------------------

void
AppImpl::SetMousePos(const glm::vec2& pos)
{
  windowContext.SetCursorPos(pos);
}

//-----------------------------------------------------------------------------

glm::vec2
AppImpl::GetMouseMotion() const
{
  return windowContext.GetMouseMotion();
}

//-----------------------------------------------------------------------------

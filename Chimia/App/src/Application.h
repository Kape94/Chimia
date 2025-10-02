#pragma once

#include "AppDefs.h"
#include "IApp.h"

#include "InputStatus.h"
#include "UiContext.h"

#include "GlfwContext.h"

BEGIN_CHIMIA_APP_NAMESPACE

class AppImpl
  :
  /*implements*/ public IAppUtils
  ,
  /*implements*/ public IApp
{
public:
  AppImpl() = default;

private:
  // -----------------------IApp-----------------------------------
  void Run() override;

  void DefineWindowProperties(
    const WindowProperties& _windowProperties) override;

  void SetUiPresenter(IuiPresenter* uiPresenter) override;
  void SetAppInitializer(IAppInitializer* _appInitializer) override;
  void SetAppRenderer(IAppRenderer* _appRenderer) override;
  void SetAppUpdater(IAppUpdater* _appUpdater) override;
  void SetAppCleanup(IAppCleanup* _appCleanup) override;
  // -----------------------IApp-----------------------------------

  // -----------------------IAppUtils------------------------------
  bool IsPressed(const Key key) override;
  bool IsPressed(const MouseButton mouseButton) override;

  bool IsReleased(const Key key) override;
  bool IsReleased(const MouseButton mouseButton) override;

  bool IsHold(const Key key) override;
  bool IsHold(const MouseButton mouseButton) override;

  void Exit() override;

  glm::vec2 GetMousePos() const override;
  void SetMousePos(const glm::vec2& pos) override;
  glm::vec2 GetMouseMotion() const override;
  // -----------------------IAppUtils------------------------------

  void Initialize();
  void InitializeContext();

  void InitializeApp();
  bool ShouldKeepRunning() const;
  void ProcessFrame();
  void BeginFrame();
  void PresentUI();
  void Update();
  void Render();
  void EndFrame();
  void Cleanup();

  GlfwContext windowContext;
  UiContext uiContext;

  InputStatus inputStatus;

  WindowProperties windowProperties;

  IAppInitializer* appInitializer = nullptr;
  IAppRenderer* appRenderer = nullptr;
  IAppUpdater* appUpdater = nullptr;
  IAppCleanup* appCleanup = nullptr;

  bool isRunning = true;
};

END_CHIMIA_APP_NAMESPACE

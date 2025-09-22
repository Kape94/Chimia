#pragma once

#include "AppToolkitDefs.h"
#include "IAppCleanup.h"
#include "IAppInitializer.h"
#include "IAppRenderer.h"
#include "IAppUpdater.h"
#include "IuiPresenter.h"
#include "WindowProperties.h"

_BEGIN_APP_TOOLKIT_NAMESPACE

class IApp
{
public:
  virtual ~IApp() = default;

  virtual void Run() = 0;

  virtual void DefineWindowProperties(
    const WindowProperties& windowProperties) = 0;

  template<class AppImpl>
  void SetImpl(AppImpl* impl)
  {
    if (auto uiPresenter = dynamic_cast<IuiPresenter*>(impl)) {
      SetUiPresenter(uiPresenter);
    }
    if (auto initializer = dynamic_cast<IAppInitializer*>(impl)) {
      SetAppInitializer(initializer);
    }
    if (auto renderer = dynamic_cast<IAppRenderer*>(impl)) {
      SetAppRenderer(renderer);
    }
    if (auto updater = dynamic_cast<IAppUpdater*>(impl)) {
      SetAppUpdater(updater);
    }
    if (auto cleanup = dynamic_cast<IAppCleanup*>(impl)) {
      SetAppCleanup(cleanup);
    }
  }

private:
  virtual void SetUiPresenter(IuiPresenter* uiPresenter) = 0;
  virtual void SetAppInitializer(IAppInitializer* _appInitializer) = 0;
  virtual void SetAppRenderer(IAppRenderer* _appRenderer) = 0;
  virtual void SetAppUpdater(IAppUpdater* _appUpdater) = 0;
  virtual void SetAppCleanup(IAppCleanup* _appCleanup) = 0;
};

_END_APP_TOOLKIT_NAMESPACE

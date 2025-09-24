#include "AppToolkit/AppToolkit.h"

//-------------------------------------------------------------------

class HelloWorldUI
  :
  /*implements*/ public AppToolkit::IuiPresenter
{
public:
  void Present() override
  {
    ImGui::Begin("Hello world");
    ImGui::Text("HellooOoOo \\o/");
    ImGui::End();
  }
};

//-------------------------------------------------------------------

int
main()
{
  AppToolkit::IApp& app = AppToolkit::GetApp();

  HelloWorldUI helloUI;
  app.SetImpl(&helloUI);

  app.Run();

  return 0;
}

//-------------------------------------------------------------------

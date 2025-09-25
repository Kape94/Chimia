#include "App/App.h"

//-------------------------------------------------------------------

class HelloWorldUI
  :
  /*implements*/ public Chimia::App::IuiPresenter
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
  Chimia::App::IApp& app = Chimia::App::GetApp();

  HelloWorldUI helloUI;
  app.SetImpl(&helloUI);

  app.Run();

  return 0;
}

//-------------------------------------------------------------------

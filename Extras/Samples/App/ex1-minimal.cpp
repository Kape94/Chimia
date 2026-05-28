#include "App/App.h"

int
main()
{
  Chimia::App::IApp& app = Chimia::App::GetApp();
  app.Run();

  return 0;
}
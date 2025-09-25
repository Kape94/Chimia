#include "UiContext.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

//-----------------------------------------------------------------------------

USING_CHIMIA_APP_NAMESPACE

//-----------------------------------------------------------------------------

void
UiContext::Init(GLFWwindow* window)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true /*install_callbacks*/);
  ImGui_ImplOpenGL3_Init("#version 330" /*glsl_version*/);
}

//-----------------------------------------------------------------------------

void
UiContext::SetPresenter(IuiPresenter* _presenter)
{
  uiPresenter = _presenter;
}

//-----------------------------------------------------------------------------

void
UiContext::PresentUI()
{
  if (uiPresenter == nullptr) {
    return;
  }

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  uiPresenter->Present();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

//-----------------------------------------------------------------------------

void
UiContext::Cleanup()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

//-----------------------------------------------------------------------------

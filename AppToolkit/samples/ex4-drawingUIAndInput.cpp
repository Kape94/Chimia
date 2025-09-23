#include "AppToolkit/AppToolkit.h"

#include <glm/vec3.hpp>

//-------------------------------------------------------------------

class ColorUI
  :
  /*implements*/ public AppToolkit::IuiPresenter
{
public:
  void Present() override
  {
    ImGui::Begin("Vertex color dialog");
    ImGui::SliderFloat("v1 red", &v1Color.r, 0.0f, 1.0f);
    ImGui::SliderFloat("v1 green", &v1Color.g, 0.0f, 1.0f);
    ImGui::SliderFloat("v1 blue", &v1Color.b, 0.0f, 1.0f);

    ImGui::SliderFloat("v2 red", &v2Color.r, 0.0f, 1.0f);
    ImGui::SliderFloat("v2 green", &v2Color.g, 0.0f, 1.0f);
    ImGui::SliderFloat("v2 blue", &v2Color.b, 0.0f, 1.0f);

    ImGui::SliderFloat("v3 red", &v3Color.r, 0.0f, 1.0f);
    ImGui::SliderFloat("v3 green", &v3Color.g, 0.0f, 1.0f);
    ImGui::SliderFloat("v3 blue", &v3Color.b, 0.0f, 1.0f);
    ImGui::End();
  }

  const glm::vec3& GetV1Color() const { return v1Color; }
  const glm::vec3& GetV2Color() const { return v2Color; }
  const glm::vec3& GetV3Color() const { return v3Color; }

  void Reset()
  {
    v1Color = { 1.0f, 0.0f, 0.0f };
    v2Color = { 0.0f, 1.0f, 0.0f };
    v3Color = { 0.0f, 0.0f, 1.0f };
  }

private:
  glm::vec3 v1Color{ 1.0f, 0.0f, 0.0f };
  glm::vec3 v2Color{ 0.0f, 1.0f, 0.0f };
  glm::vec3 v3Color{ 0.0f, 0.0f, 1.0f };
};

//-------------------------------------------------------------------

class SimpleTriangleDrawer
  :
  /*implements*/ public AppToolkit::IAppRenderer
{
public:
  SimpleTriangleDrawer() = delete;

  SimpleTriangleDrawer(const ColorUI& _colorUI)
    : colorUI(_colorUI)
  {
  }

  void Render() override
  {
    const glm::vec3& v1Col = colorUI.GetV1Color();
    const glm::vec3& v2Col = colorUI.GetV2Color();
    const glm::vec3& v3Col = colorUI.GetV3Color();

    glBegin(GL_TRIANGLES);
    glColor3f(v1Col.r, v1Col.g, v1Col.b);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glColor3f(v2Col.r, v2Col.g, v2Col.b);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glColor3f(v3Col.r, v3Col.g, v3Col.b);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glEnd();
  }

private:
  const ColorUI& colorUI;
};

//-------------------------------------------------------------------

class InputHandler
  :
  /*implements*/ public AppToolkit::IAppUpdater
{
public:
  InputHandler() = delete;

  InputHandler(ColorUI& _colorUI)
    : colorUI(_colorUI)
  {
  }

  void Update(AppToolkit::IAppUtils& appUtils) override
  {
    if (appUtils.IsPressed(AppToolkit::Key::R)) {
      colorUI.Reset();
    }
    if (appUtils.IsPressed(AppToolkit::Key::ESC)) {
      appUtils.Exit();
    }
  }

private:
  ColorUI& colorUI;
};

//-------------------------------------------------------------------

int
main()
{
  AppToolkit::IApp& app = AppToolkit::GetApp();

  ColorUI colorUI;
  SimpleTriangleDrawer drawer(colorUI);
  InputHandler inputHandler(colorUI);

  app.SetImpl(&colorUI);
  app.SetImpl(&drawer);
  app.SetImpl(&inputHandler);

  AppToolkit::WindowProperties windowProps;
  windowProps.width = 1280;
  windowProps.height = 960;
  windowProps.fullscreen = true;
  app.DefineWindowProperties(windowProps);

  app.Run();

  return 0;
}

//-------------------------------------------------------------------

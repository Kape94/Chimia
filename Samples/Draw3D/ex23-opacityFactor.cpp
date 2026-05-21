#include "Draw3D/Draw3D.h"
#include "Draw3D/Resources.h"
#include "Draw3D/Triangle.h"
#include "Draw3D/Types.h"
#include "Utils/SamplesUtils.h"
#include "Utils/Window.h"

// ----------------------------------------------------------------------------

int
main()
{
  Window w(1280, 960, "Draw3D ex23");

  Chimia::Draw3D::Initialize();

  SamplesUtils::InitRandom();

  const Chimia::Draw3D::ResourceGroupID drawResource =
    Chimia::Draw3D::CreateResourceGroup();
  const Chimia::Draw3D::OpacityFactorID opacity =
    Chimia::Draw3D::CreateOpacityFactor(0.3f);

  Chimia::Draw3D::AddResourceToGroup(opacity, drawResource);

  float alpha = 0.3f;
  while (!w.ShouldClose()) {
    Chimia::Draw3D::ClearScreen();

    constexpr float opaqueObjectDepth = 0.0f;
    Chimia::Draw3D::Triangle(
      Chimia::Draw3D::VertexPC{ { 0.0f, 0.0f, opaqueObjectDepth },
                                { 1.0f, 0.0f, 0.0f, 1.0f } },
      Chimia::Draw3D::VertexPC{ { 1.0f, 0.0f, opaqueObjectDepth },
                                { 0.0f, 1.0f, 0.0f, 1.0f } },
      Chimia::Draw3D::VertexPC{ { 0.0f, 1.0f, opaqueObjectDepth },
                                { 0.0f, 0.0f, 1.0f, 1.0f } });

    constexpr float blueGlassDepth = 0.5f;
    Chimia::Draw3D::Triangle(
      Chimia::Draw3D::VertexPC{ { -1.0f, 0.0f, blueGlassDepth },
                                { 0.0f, 0.0f, 1.0f, 1.0f } },
      Chimia::Draw3D::VertexPC{ { 1.0f, 0.0f, blueGlassDepth },
                                { 0.0f, 0.0f, 1.0f, 1.0f } },
      Chimia::Draw3D::VertexPC{ { -1.0f, 1.0f, blueGlassDepth },
                                { 0.0f, 0.0f, 1.0f, 1.0f } },
      drawResource);
    constexpr float greenGlassDepth = -0.5f;
    Chimia::Draw3D::Triangle(
      Chimia::Draw3D::VertexPC{ { -1.0f, 0.0f, greenGlassDepth },
                                { 0.0f, 1.0f, 0.0f, 1.0f } },
      Chimia::Draw3D::VertexPC{ { 1.0f, 0.0f, greenGlassDepth },
                                { 0.0f, 1.0f, 0.0f, 1.0f } },
      Chimia::Draw3D::VertexPC{ { 1.0f, 0.5f, greenGlassDepth },
                                { 0.0f, 1.0f, 0.0f, 1.0f } },
      drawResource);

    alpha += 0.01f;
    if (alpha > 0.9f) {
      alpha = 0.05f;
    }

    Chimia::Draw3D::UpdateOpacityFactor(opacity, alpha);
    Chimia::Draw3D::Flush();

    w.Swap();
    w.PollEvents();
    SamplesUtils::SyncForTargetFPS(30);
  }
  return 0;
}

// ----------------------------------------------------------------------------

#include "Draw3D/Draw3D.h"
#include "Draw3D/Resources.h"
#include "Draw3D/Triangle.h"
#include "Draw3D/Types.h"
#include "Utils/ExtrasUtils.h"
#include "Utils/Window.h"

// ----------------------------------------------------------------------------

class SampleVertex
{
public:
  SampleVertex(const glm::vec3& pos);

  void UpdateColor();

  const Chimia::Draw3D::VertexPC& GetVertex() const;

private:
  Chimia::Draw3D::VertexPC m_vertex;
  glm::vec3 m_direction;
  float m_changeVelocity;
};

SampleVertex::SampleVertex(const glm::vec3& pos)
{
  using namespace ExtrasUtils;

  m_vertex.position = pos;
  m_vertex.color = {
    NormalizedRand(), NormalizedRand(), NormalizedRand(), 1.0f
  };
  m_direction = { NormalizedRand(), NormalizedRand(), NormalizedRand() };
  m_changeVelocity = 0.005f + NormalizedRand() * 0.015f;
}

void
SampleVertex::UpdateColor()
{
  glm::vec4& color = m_vertex.color;
  if (color.r >= 1.0f)
    m_direction.r = -1.0f;
  if (color.r <= 0.0f)
    m_direction.r = 1.0f;
  if (color.g >= 0.5f)
    m_direction.g = -1.0f;
  if (color.g <= 0.0f)
    m_direction.g = 1.0f;
  if (color.b >= 1.0f)
    m_direction.b = -1.0f;
  if (color.b <= 0.0f)
    m_direction.b = 1.0f;

  color += glm::vec4(m_direction, 0.0f) * m_changeVelocity;
}

const Chimia::Draw3D::VertexPC&
SampleVertex::GetVertex() const
{
  return m_vertex;
}

// ----------------------------------------------------------------------------

int
main()
{
  Window w(1280, 960, "Draw3D ex24");

  Chimia::Draw3D::Initialize();

  SampleVertex v1({ -0.5f, -0.5f, 0.0f });
  SampleVertex v2({ 0.5f, -0.5f, 0.0f });
  SampleVertex v3({ 0.0f, 0.5f, 0.0f });

  const Chimia::Draw3D::ResourceGroupID drawResource =
    Chimia::Draw3D::CreateResourceGroup();
  const Chimia::Draw3D::MixtureColorID mixtureColor =
    Chimia::Draw3D::CreateMixtureColor({ 0.0f, 0.0f, 1.0f });

  Chimia::Draw3D::AddResourceToGroup(mixtureColor, drawResource);

  float pseudoTime = 0.0f;
  while (!w.ShouldClose()) {

    Chimia::Draw3D::ClearScreen();

    if (pseudoTime > 3.0f) {
      Chimia::Draw3D::Triangle(
        v1.GetVertex(), v2.GetVertex(), v3.GetVertex(), drawResource);
    } else {
      Chimia::Draw3D::Triangle(v1.GetVertex(), v2.GetVertex(), v3.GetVertex());
    }

    Chimia::Draw3D::Flush();

    w.Swap();
    w.PollEvents();

    v1.UpdateColor();
    v2.UpdateColor();
    v3.UpdateColor();

    pseudoTime += 0.01f;
    if (pseudoTime > 6.0f) {
      pseudoTime = 0.0f;
      Chimia::Draw3D::UpdateMixtureColor(
        mixtureColor, { 0.0f, 0.0f, ExtrasUtils::NormalizedRand() });
    }
  }
  return 0;
}

// ----------------------------------------------------------------------------

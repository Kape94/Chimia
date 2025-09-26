#include "Draw3D/Draw3D.h"
#include "Draw3D/Triangle.h"
#include "Utils/SamplesUtils.h"
#include "Utils/Window.h"

// ----------------------------------------------------------------------------

class SampleVertex
{
public:
  SampleVertex(const glm::vec3& pos);

  void UpdateColor();

  const glm::vec3& GetPos() const;
  const glm::vec3& GetColor() const;

private:
  glm::vec3 m_pos;
  glm::vec3 m_color;
  glm::vec3 m_direction;
  float m_changeVelocity;
};

SampleVertex::SampleVertex(const glm::vec3& pos)
  : m_pos(pos)
{
  using namespace SamplesUtils;

  m_color = { NormalizedRand(), NormalizedRand(), NormalizedRand() };
  m_direction = { NormalizedRand(), NormalizedRand(), NormalizedRand() };
  m_changeVelocity = 0.005f + NormalizedRand() * 0.015f;
}

void
SampleVertex::UpdateColor()
{
  if (m_color.r >= 1.0f)
    m_direction.r = -1.0f;
  if (m_color.r <= 0.0f)
    m_direction.r = 1.0f;
  if (m_color.g >= 0.5f)
    m_direction.g = -1.0f;
  if (m_color.g <= 0.0f)
    m_direction.g = 1.0f;
  if (m_color.b >= 1.0f)
    m_direction.b = -1.0f;
  if (m_color.b <= 0.0f)
    m_direction.b = 1.0f;

  m_color = m_color + m_direction * m_changeVelocity;
}

const glm::vec3&
SampleVertex::GetPos() const
{
  return m_pos;
}

const glm::vec3&
SampleVertex::GetColor() const
{
  return m_color;
}

// ----------------------------------------------------------------------------

int
main()
{
  Window w(1280, 960, "Draw3D ex2");

  Chimia::Draw3D::Initialize();

  SampleVertex v1({ -0.5f, -0.5f, 0.0f });
  SampleVertex v2({ 0.5f, -0.5f, 0.0f });
  SampleVertex v3({ 0.0f, 0.5f, 0.0f });

  while (!w.ShouldClose()) {

    Chimia::Draw3D::ClearScreen();

    Chimia::Draw3D::Triangle(v1.GetPos(),
                             v1.GetColor(),
                             v2.GetPos(),
                             v2.GetColor(),
                             v3.GetPos(),
                             v3.GetColor());

    Chimia::Draw3D::Flush();

    w.Swap();
    w.PollEvents();

    v1.UpdateColor();
    v2.UpdateColor();
    v3.UpdateColor();
  }
  return 0;
}

// ----------------------------------------------------------------------------

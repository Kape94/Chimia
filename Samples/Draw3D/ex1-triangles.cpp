#include "Draw3D/Draw3D.h"
#include "Draw3D/Triangle.h"
#include "Draw3D/Types.h"
#include "Utils/SamplesUtils.h"
#include "Utils/Window.h"

#include <vector>

// ----------------------------------------------------------------------------

constexpr unsigned NUM_TRIANGLES = 2000;

std::vector<glm::vec3> initialDirs{ { -1.0f, -1.0f, 0.0f },
                                    { -1.0f, 1.0f, 0.0f },
                                    { 1.0f, 1.0f, 0.0f },
                                    { 1.0f, -1.0f, 0.0f } };

// ----------------------------------------------------------------------------
// Triangle class
// ----------------------------------------------------------------------------

class Triangle
{
public:
  Triangle();

  const glm::vec3& Pos() const;

  void Move();
  void Draw() const;

private:
  glm::vec3 m_pos;
  glm::vec3 m_dir;
  glm::vec3 m_col;
  float m_velocity;
  float m_size;
};

Triangle::Triangle()
{
  using namespace SamplesUtils;

  m_pos = glm::vec3{ -1.0f + NormalizedRand() * 2.0f,
                     -1.0f + NormalizedRand() * 2.0f,
                     0.0f };
  m_col = glm::vec3{ 0.0f + NormalizedRand() * 1.0f,
                     0.0f + NormalizedRand() * 1.0f,
                     0.0f + NormalizedRand() * 1.0f };
  m_size = 0.01f + 0.02f * NormalizedRand();

  const unsigned randomPos = Rand() % initialDirs.size();
  m_dir = initialDirs[randomPos];
  m_velocity = 0.005f + 0.015f * NormalizedRand();
}

const glm::vec3&
Triangle::Pos() const
{
  return m_pos;
}

void
Triangle::Move()
{
  m_pos += m_dir * m_velocity;
  if (m_pos.x < -1.0f)
    m_dir.x = 1.0f;
  if (m_pos.x > 1.0f)
    m_dir.x = -1.0f;
  if (m_pos.y < -1.0f)
    m_dir.y = 1.0f;
  if (m_pos.y > 1.0f)
    m_dir.y = -1.0f;
}

void
Triangle::Draw() const
{
  glm::vec3 p1Dir(-1.0f, -1.0f, 0.0f);
  p1Dir = p1Dir / (float)p1Dir.length();

  glm::vec3 p2Dir(1.0f, -1.0f, 0.0f);
  p2Dir = p2Dir / (float)p2Dir.length();

  glm::vec3 p3Dir(0.0f, 1.0f, 0.0f);
  p3Dir = p3Dir / (float)p3Dir.length();

  const glm::vec3 p1(m_pos + p1Dir * m_size);
  const glm::vec3 p2(m_pos + p2Dir * m_size);
  const glm::vec3 p3(m_pos + p3Dir * m_size);

  Chimia::Draw3D::Triangle(Chimia::Draw3D::VertexPC{ p1, m_col },
                           Chimia::Draw3D::VertexPC{ p2, m_col },
                           Chimia::Draw3D::VertexPC{ p3, m_col });
}

// ----------------------------------------------------------------------------

std::vector<Triangle>
CreateTriangles()
{
  std::vector<Triangle> triangles;
  for (int i = 0; i < NUM_TRIANGLES; ++i) {
    triangles.push_back(Triangle());
  }
  return triangles;
}

// ----------------------------------------------------------------------------
// main
// ----------------------------------------------------------------------------

int
main()
{
  Window w(1280, 960, "Draw3D ex1");

  Chimia::Draw3D::Initialize();

  SamplesUtils::InitRandom();

  auto triangles = CreateTriangles();
  while (!w.ShouldClose()) {

    for (Triangle& t : triangles) {
      t.Move();
    }

    Chimia::Draw3D::ClearScreen();
    for (const Triangle& t : triangles) {
      t.Draw();
    }

    Chimia::Draw3D::Flush();

    w.Swap();
    w.PollEvents();
    SamplesUtils::SyncForTargetFPS(30);
  }
  return 0;
}

// ----------------------------------------------------------------------------

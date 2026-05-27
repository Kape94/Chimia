#pragma once

// ----------------------------------------------------------------------------

#include "MediaNamespaceDefs.h"

#include <vector>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_MEDIA_NAMESPACE

struct Vector2f
{
  float x;
  float y;
};

struct Vector3f
{
  float x;
  float y;
  float z;
};

struct Color4f
{
  float r;
  float g;
  float b;
  float a;
};

struct Mesh
{
  std::vector<Vector3f> vertices;
  std::vector<Color4f> colors;
  std::vector<Vector3f> normals;
  std::vector<Vector2f> texCoords;
  std::vector<unsigned> indices;
};

struct MeshAttributesToInclude
{
  bool includeColors = false;
  bool includeNormals = false;
  bool includeTexCoords = false;
};

struct BufferData
{
  std::vector<float> vertexData;
  std::vector<unsigned> indices;
};

END_CHIMIA_MEDIA_NAMESPACE

// ----------------------------------------------------------------------------
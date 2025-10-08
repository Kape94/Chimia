#pragma once

#include "ImporterNamespaceDefs.h"

#include <string>
#include <vector>

BEGIN_CHIMIA_IMPORTER_NAMESPACE

struct vector2f
{
  float x;
  float y;
};

struct vector3f
{
  float x;
  float y;
  float z;
};

struct vector4f
{
  float x;
  float y;
  float z;
  float w;
};

struct Mesh
{
  std::vector<vector3f> vertices;
  std::vector<vector4f> colors;
  std::vector<vector3f> normals;
  std::vector<vector2f> texCoords;
  std::vector<unsigned> indices;
};

std::vector<Mesh>
ImportMeshes(const std::string& path);

END_CHIMIA_IMPORTER_NAMESPACE
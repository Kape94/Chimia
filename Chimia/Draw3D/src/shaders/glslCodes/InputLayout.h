#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace ShaderCodes {

namespace VertexInputLayout {

inline const char* P3C3 = R"(
    layout (location = 0) in vec3 vertexPos;
    layout (location = 1) in vec3 vertexColor;
)";

inline const char* P3C3Instanced = R"(
    layout (location = 0) in vec3 vertexPos;
    layout (location = 1) in vec3 vertexColor;
    layout (location = 2) in mat4 instanceTransform;
)";

inline const char* P3N3 = R"(
  layout (location = 0) in vec3 vertexPos;
  layout (location = 1) in vec3 vertexNorm;
)";

inline const char* P3N3Instanced = R"(
    layout (location = 0) in vec3 vertexPos;
    layout (location = 1) in vec3 vertexNorm;
    layout (location = 2) in mat4 instanceTransform;
)";

inline const char* P3C3N3 = R"(
    layout (location = 0) in vec3 vertexPos;
    layout (location = 1) in vec3 vertexColor;
    layout (location = 2) in vec3 vertexNorm;
  )";

inline const char* P3C3N3Instanced = R"(
    layout (location = 0) in vec3 vertexPos;
    layout (location = 1) in vec3 vertexColor;
    layout (location = 2) in vec3 vertexNorm;
    layout (location = 3) in mat4 instanceTransform;
  )";

inline const char* P3T2 = R"(
    layout (location = 0) in vec3 vertexPos;
    layout (location = 1) in vec2 vertexTexCoord;
  )";

inline const char* P3T2Instanced = R"(
    layout (location = 0) in vec3 vertexPos;
    layout (location = 1) in vec2 vertexTexCoord;
    layout (location = 2) in mat4 instanceTransform;
  )";

inline const char* P3C3T2 = R"(
    layout (location = 0) in vec3 vertexPos;
    layout (location = 1) in vec3 vertexColor;
    layout (location = 2) in vec2 vertexTexCoord;
  )";

inline const char* P3C3T2Instanced = R"(
    layout (location = 0) in vec3 vertexPos;
    layout (location = 1) in vec3 vertexColor;
    layout (location = 2) in vec2 vertexTexCoord;
    layout (location = 3) in mat4 instanceTransform;
  )";

inline const char* P3N3T2 = R"(
    layout (location = 0) in vec3 vertexPos;
    layout (location = 1) in vec3 vertexNorm;
    layout (location = 2) in vec2 vertexTexCoord;
  )";

inline const char* P3N3T2Instanced = R"(
    layout (location = 0) in vec3 vertexPos;
    layout (location = 1) in vec3 vertexNorm;
    layout (location = 2) in vec2 vertexTexCoord;
    layout (location = 3) in mat4 instanceTransform;
  )";

inline const char* P3C3N3T2 = R"(
    layout (location = 0) in vec3 vertexPos;
    layout (location = 1) in vec3 vertexColor;
    layout (location = 2) in vec3 vertexNorm;
    layout (location = 3) in vec2 vertexTexCoord;
  )";

inline const char* P3C3N3T2Instanced = R"(
    layout (location = 0) in vec3 vertexPos;
    layout (location = 1) in vec3 vertexColor;
    layout (location = 2) in vec3 vertexNorm;
    layout (location = 3) in vec2 vertexTexCoord;
    layout (location = 4) in mat4 instanceTransform;
  )";

}

namespace FragmentInputLayout {

inline const char* C3 = R"(
    in vec3 fragmentColor;
)";

inline const char* P3N3 = R"(
    in vec3 fragmentPos;
    in vec3 fragmentNorm;
)";

inline const char* P3C3N3 = R"(
    in vec3 fragmentPos;
    in vec3 fragmentColor;
    in vec3 fragmentNorm;
)";

inline const char* T2 = R"(
    in vec2 fragmentTexCoord;
)";

inline const char* C3T2 = R"(
    in vec3 fragmentColor;
    in vec2 fragmentTexCoord;
)";

inline const char* T2Light = R"(
    in vec2 fragmentTexCoord;
    in vec3 fragmentLightColor;
)";

inline const char* P3N3T2 = R"(
    in vec3 fragmentPos;
    in vec3 fragmentNorm;
    in vec2 fragmentTexCoord;
)";

inline const char* P3C3N3T2 = R"(
    in vec3 fragmentPos;
    in vec3 fragmentColor;
    in vec3 fragmentNorm;
    in vec2 fragmentTexCoord;
)";

}
}

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
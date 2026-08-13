#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include <string>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace DataNames {

namespace ShaderUniforms {
inline const std::string MIXTURE_COLOR = "u_mixtureColor";
inline const std::string OPACITY = "u_opacity";
inline const std::string TEXTURE = "u_tex";
inline const std::string MATERIAL = "u_material";

inline const std::string HAS_VERTEX_COLOR = "u_hasVertexColor";
inline const std::string HAS_NORMAL = "u_hasNormal";
inline const std::string HAS_TEXCOORD = "u_hasTexCoord";
inline const std::string IS_INSTANCED = "u_isInstanced";
inline const std::string HAS_MATERIAL = "u_hasMaterial";
inline const std::string HAS_TEXTURE = "u_hasTexture";
inline const std::string LIGHTNING_MODEL = "u_lightningModel";

inline const std::string CAMERA_TRANSFORM = "u_cameraTransform";
inline const std::string VIEW_POSITION = "u_viewPosition";

inline const std::string DIRECTIONAL_LIGHTS_ARRAY = "u_directionalLights";
inline const std::string N_DIRECTIONAL_LIGHTS = "u_nDirectionalLights";
inline const std::string POINT_LIGHTS_ARRAY = "u_pointLights";
inline const std::string N_POINT_LIGHTS = "u_nPointLights";

inline const std::string IS_TRANSPARENT_RENDERING = "u_isTransparentRendering";
inline const std::string IS_TRANSITION_RENDERING = "u_isTransitionRendering";
}

namespace ShaderInputs {
inline const std::string VERTEX_POS = "a_vertexPos";
inline const std::string VERTEX_COLOR = "a_vertexColor";
inline const std::string VERTEX_NORMAL = "a_vertexNorm";
inline const std::string VERTEX_TEX_COORD = "a_vertexTexCoord";
inline const std::string VERTEX_TARGET_POS = "a_targetVertexPos";
inline const std::string VERTEX_TARGET_COLOR = "a_targetVertexColor";
inline const std::string VERTEX_TARGET_NORMAL = "a_targetVertexNorm";
inline const std::string VERTEX_TARGET_TEX_COORD = "a_targetVertexTexCoord";
inline const std::string INSTANCE_TRANSITION_INTERPOLATION =
  "a_transitionInterpolation";
inline const std::string INSTANCE_TRANSFORM = "a_instanceTransform";
}

namespace BufferData {
inline const std::string VERTEX_POS = "position";
inline const std::string VERTEX_COLOR = "color";
inline const std::string VERTEX_NORMAL = "normal";
inline const std::string VERTEX_TEX_COORD = "texCoord";
inline const std::string INSTANCE_INTERPOLATION = "interpolation";
inline const std::string INSTANCE_TRANSFORM = "transform";
}
}

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
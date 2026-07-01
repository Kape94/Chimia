#pragma once

// ----------------------------------------------------------------------------

#include "Draw3DNamespaceDefs.h"

#include <string>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

namespace ShaderUniformsNames {

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

}

// ----------------------------------------------------------------------------

END_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
#include "VertexLayoutAttributes.h"

#include "Core/Diagnostics.h"

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------

/*
  layout (location = 0) in vec3 vertexPos;
  layout (location = 1) in vec3 vertexColor;
  layout (location = 2) in vec3 vertexNorm;
  layout (location = 3) in vec2 vertexTexCoord;
  layout (location = 4) in mat4 instanceTransform;
  */

VertexLayoutAttributes
CHIMIA_DRAW3D_NAMESPACE_NAME::GetAttributesForLayout(
  const eVertexLayout& layout)
{
  switch (layout) {
    case eVertexLayout::POSITION3_COLOR3: {
      return { { Rendering::ShaderAttribute::Float(0 /*pos*/, 3),
                 Rendering::ShaderAttribute::Float(1 /*color*/, 3) },
               { Rendering::ShaderAttribute::Float(4 /*transform*/, 4),
                 Rendering::ShaderAttribute::Float(5 /*transform*/, 4),
                 Rendering::ShaderAttribute::Float(6 /*transform*/, 4),
                 Rendering::ShaderAttribute::Float(7 /*transform*/, 4) } };
    }
    case eVertexLayout::POSITION3_NORMAL3: {
      return { { Rendering::ShaderAttribute::Float(0 /*pos*/, 3),
                 Rendering::ShaderAttribute::Float(2 /*normal*/, 3) },
               { Rendering::ShaderAttribute::Float(4 /*transform*/, 4),
                 Rendering::ShaderAttribute::Float(5 /*transform*/, 4),
                 Rendering::ShaderAttribute::Float(6 /*transform*/, 4),
                 Rendering::ShaderAttribute::Float(7 /*transform*/, 4) } };
    }
    case eVertexLayout::POSITION3_TEXCOORD2: {
      return { { Rendering::ShaderAttribute::Float(0 /*pos*/, 3),
                 Rendering::ShaderAttribute::Float(3 /*texCoord*/, 2) },
               { Rendering::ShaderAttribute::Float(4 /*transform*/, 4),
                 Rendering::ShaderAttribute::Float(5 /*transform*/, 4),
                 Rendering::ShaderAttribute::Float(6 /*transform*/, 4),
                 Rendering::ShaderAttribute::Float(7 /*transform*/, 4) } };
    }
    case eVertexLayout::POSITION3_COLOR3_NORMAL3: {
      return { { Rendering::ShaderAttribute::Float(0 /*pos*/, 3),
                 Rendering::ShaderAttribute::Float(1 /*color*/, 3),
                 Rendering::ShaderAttribute::Float(2 /*normal*/, 3) },
               { Rendering::ShaderAttribute::Float(4 /*transform*/, 4),
                 Rendering::ShaderAttribute::Float(5 /*transform*/, 4),
                 Rendering::ShaderAttribute::Float(6 /*transform*/, 4),
                 Rendering::ShaderAttribute::Float(7 /*transform*/, 4) } };
    }
    case eVertexLayout::POSITION3_COLOR3_TEXCOORD2: {
      return { { Rendering::ShaderAttribute::Float(0 /*pos*/, 3),
                 Rendering::ShaderAttribute::Float(1 /*color*/, 3),
                 Rendering::ShaderAttribute::Float(3 /*texCoord*/, 2) },
               { Rendering::ShaderAttribute::Float(4 /*transform*/, 4),
                 Rendering::ShaderAttribute::Float(5 /*transform*/, 4),
                 Rendering::ShaderAttribute::Float(6 /*transform*/, 4),
                 Rendering::ShaderAttribute::Float(7 /*transform*/, 4) } };
    }
    case eVertexLayout::POSITION3_NORMAL3_TEXCOORD2: {
      return { { Rendering::ShaderAttribute::Float(0 /*pos*/, 3),
                 Rendering::ShaderAttribute::Float(2 /*normal*/, 3),
                 Rendering::ShaderAttribute::Float(3 /*texCoord*/, 2) },
               { Rendering::ShaderAttribute::Float(4 /*transform*/, 4),
                 Rendering::ShaderAttribute::Float(5 /*transform*/, 4),
                 Rendering::ShaderAttribute::Float(6 /*transform*/, 4),
                 Rendering::ShaderAttribute::Float(7 /*transform*/, 4) } };
    }
    case eVertexLayout::POSITION3_COLOR3_NORMAL3_TEXCOORD2: {
      return { { Rendering::ShaderAttribute::Float(0 /*pos*/, 3),
                 Rendering::ShaderAttribute::Float(1 /*color*/, 3),
                 Rendering::ShaderAttribute::Float(2 /*normal*/, 3),
                 Rendering::ShaderAttribute::Float(3 /*texCoord*/, 2) },
               { Rendering::ShaderAttribute::Float(4 /*transform*/, 4),
                 Rendering::ShaderAttribute::Float(5 /*transform*/, 4),
                 Rendering::ShaderAttribute::Float(6 /*transform*/, 4),
                 Rendering::ShaderAttribute::Float(7 /*transform*/, 4) } };
    }
    case eVertexLayout::UNDEFINED:
    default: {
      Diagnostics::Error(1, "Unable to fetch attributes for undefined layout");
      return {};
    }
  }
}

// ----------------------------------------------------------------------------
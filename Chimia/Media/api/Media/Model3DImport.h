#pragma once

// ----------------------------------------------------------------------------

#include "MediaNamespaceDefs.h"
#include "MediaTypes.h"

#include <string>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_MEDIA_NAMESPACE

std::vector<Mesh>
ImportMeshes(const std::string& path);

std::vector<BufferData>
PackBufferDataFromMeshes(const std::vector<Mesh>& meshes,
                         const MeshAttributesToInclude& attributes);

END_CHIMIA_MEDIA_NAMESPACE

// ----------------------------------------------------------------------------

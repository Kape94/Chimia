#pragma once

// ----------------------------------------------------------------------------

#include "ImporterNamespaceDefs.h"
#include "ImporterTypes.h"

#include <string>

// ----------------------------------------------------------------------------

BEGIN_CHIMIA_IMPORTER_NAMESPACE

std::vector<Mesh>
ImportMeshes(const std::string& path);

std::vector<BufferData>
PackBufferDataFromMeshes(const std::vector<Mesh>& meshes,
                         const MeshAttributesToInclude& attributes);

END_CHIMIA_IMPORTER_NAMESPACE

// ----------------------------------------------------------------------------

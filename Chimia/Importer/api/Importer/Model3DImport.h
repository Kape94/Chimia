#pragma once

#include "ImporterNamespaceDefs.h"

#include <string>
#include <vector>

BEGIN_CHIMIA_IMPORTER_NAMESPACE

void
ImportModel(const std::string& path,
            std::vector<float>& vertices,
            std::vector<float>& normals,
            std::vector<float>& texCoords,
            std::vector<unsigned>& indices);

END_CHIMIA_IMPORTER_NAMESPACE
#pragma once

#include "Draw3D/Types.h"

#include <vector>

namespace GraphicsTestsData {

std::vector<Chimia::Draw3D::VertexPCNT>
QuadPCNT(size_t quadNumber);

std::vector<Chimia::Draw3D::VertexPC>
QuadPC(size_t quadNumber);

std::vector<Chimia::Draw3D::VertexPN>
QuadPN(size_t quadNumber);

std::vector<Chimia::Draw3D::VertexPT>
QuadPT(size_t quadNumber);

std::vector<Chimia::Draw3D::VertexPCN>
QuadPCN(size_t quadNumber);

std::vector<Chimia::Draw3D::VertexPCT>
QuadPCT(size_t quadNumber);

size_t
NQuads();
}
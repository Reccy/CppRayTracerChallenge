#pragma once
#include <string>
#include "rogll/include.h"

namespace PLY
{
	ROGLL::Mesh* LoadMesh(std::string filepath, const ROGLL::VertexAttributes& layout);
}

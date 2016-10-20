#pragma once
#include "Vertex.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>

namespace Utility {
	glm::vec2 getOffsetOfUVIndex(int Index, int divisions);
	void sheetUV(Vertex (&vertices)[6], int index, int divisionsX, int divisionsY);
	void splitString(const std::string& s, char c, std::vector<std::string>& v);
}
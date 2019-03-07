#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Wall
{

	// Create square with given center, length and color
	Mesh* CreateWall(std::string name, glm::vec3 leftBottomCorner, float height, float width, glm::vec3 color, bool fill = false);

}


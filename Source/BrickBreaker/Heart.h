#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

using namespace std;

class Heart
{
public:
	Heart();
	~Heart();
	float scale = 1;
	bool scaleDown = true;
	// Create heart with given bottom tip, length and color
	Mesh* CreateHeart(std::string name, glm::vec3 tip, float unit, glm::vec3 color, bool fill = false);

};



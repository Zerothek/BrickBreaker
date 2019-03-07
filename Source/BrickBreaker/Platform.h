#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

using namespace std;

class Platform
{
	float x, y;
public:
	float width;
	Platform(float x, float y, float width);
	~Platform();
	void Platform::changeMiddle(float x);
	float leftBorder();
	float rightBorder();

	// Create square with given center, length and color
	Mesh* CreatePlatform(std::string name, glm::vec3 center, float height, float width, glm::vec3 color, bool fill = false);

};


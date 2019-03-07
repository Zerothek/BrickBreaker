#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

using namespace std;

class Ball
{
	float x, y, radius;
public:
	Ball(float x, float y, float radius);
	~Ball();

	// Create ball with given center, radius and color
	Mesh* CreateBall(std::string name, glm::vec3 middle, float radius, glm::vec3 color, bool fill = false);

};


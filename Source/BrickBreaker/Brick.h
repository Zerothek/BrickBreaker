#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

using namespace std;

class Brick
{
	float x, y, height, width, scaleX, scaleY;
	int visible;
public:
	Brick(float x, float y, float height, float width);
	~Brick();
	glm::vec4 Brick::getBorders();
	int Brick::getVisible();
	void Brick::setVisible();
	void Brick::resetVisible();
	glm::vec2 Brick::getScale();
	void Brick::setScale(float scaleX, float scaleY);


	// Create square with given center, length and color
	Mesh* CreateBrick(std::string name, glm::vec3 center, float height, float width, glm::vec3 color, bool fill = false);

};


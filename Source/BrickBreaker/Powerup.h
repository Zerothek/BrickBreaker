#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

using namespace std;

class Powerup
{
	int visible;
public:
	float x, y, height, width, scaleX, scaleY;
	Powerup(float x, float y, float height, float width);
	~Powerup();
	glm::vec4 Powerup::getBorders();
	int Powerup::getVisible();
	void Powerup::setVisible();
	void Powerup::resetVisible();
	glm::vec2 Powerup::getScale();
	void Powerup::setScale(float scaleX, float scaleY);
	void Powerup::setCoordinates(float x, float y);



	// Create square with given bottom left corner, length and color
	Mesh* CreatePowerup(std::string name, glm::vec3 leftBottomCorner, float height, float width, bool fill = false);
	Mesh* CreateShield(std::string name, glm::vec3 leftBottomCorner, float height, float width, bool fill = false);

};


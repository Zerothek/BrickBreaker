#include "Powerup.h"

#include <Core/Engine.h>

using namespace std;

Powerup::Powerup(float x, float y, float height, float width) {
	this->x = x;
	this->y = y;
	this->height = height;
	this->width = width;
	this->visible = 0;
	this->scaleX = 1.f;
	this->scaleY = 1.f;
}

void Powerup::setScale(float scaleX, float scaleY) {
	this->scaleX = scaleX;
	this->scaleY = scaleY;
}

void Powerup::setCoordinates(float x, float y) {
	this->x = x;
	this->y = y;
}

glm::vec2 Powerup::getScale() {
	return glm::vec2(this->x, this->y);
}

int Powerup::getVisible() {
	return this->visible;
}

void Powerup::setVisible() {
	this->visible = 1;
}

void Powerup::resetVisible() {
	this->visible = 0;
}

Powerup::~Powerup() {

}

glm::vec4 Powerup::getBorders() {
	return glm::vec4(this->y - this->height / 2, this->y + this->height / 2, this->x - this->width / 2, this->x + this->width / 2);
	//return the 4 borderns of the powerup square
}

Mesh* Powerup::CreatePowerup(std::string name, glm::vec3 middle, float height, float width, bool fill)
{
	float u = height / 9;
	glm::vec3 color = glm::vec3(1, 0.59, 0);
	glm::vec3 color1 = glm::vec3(0.36, 0.17, 0);
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(middle - glm::vec3(width / 2, height / 2, 0), color),
		VertexFormat(middle + glm::vec3(width, 0, 0) - glm::vec3(width / 2, height / 2, 0), color),
		VertexFormat(middle + glm::vec3(width, height, 0) - glm::vec3(width / 2, height / 2, 0), color),
		VertexFormat(middle + glm::vec3(0, height, 0) - glm::vec3(width / 2, height / 2, 0), color),
		VertexFormat(middle + glm::vec3(0, 3.5 * u, 0), color1),
		VertexFormat(middle + glm::vec3(2 * u, 1.5 * u, 0), color1),
		VertexFormat(middle + glm::vec3(2 * u, 0.5 * u, 0), color1),
		VertexFormat(middle + glm::vec3(0, 2.5 * u, 0), color1),
		VertexFormat(middle + glm::vec3(-2 * u, 0.5 * u, 0), color1),
		VertexFormat(middle + glm::vec3(-2 * u, 1.5 * u, 0), color1),
		VertexFormat(middle + glm::vec3(0, 3.5 * u, 0) - glm::vec3(0, 2 * u, 0), color1),
		VertexFormat(middle + glm::vec3(2 * u, 1.5 * u, 0) - glm::vec3(0, 2 * u, 0), color1),
		VertexFormat(middle + glm::vec3(2 * u, 0.5 * u, 0) - glm::vec3(0, 2 * u, 0), color1),
		VertexFormat(middle + glm::vec3(0, 2.5 * u, 0) - glm::vec3(0, 2 * u, 0), color1),
		VertexFormat(middle + glm::vec3(-2 * u, 0.5 * u, 0) - glm::vec3(0, 2 * u, 0), color1),
		VertexFormat(middle + glm::vec3(-2 * u, 1.5 * u, 0) - glm::vec3(0, 2 * u, 0), color1),
		VertexFormat(middle + glm::vec3(0, 3.5 * u, 0) - glm::vec3(0, 4 * u, 0), color1),
		VertexFormat(middle + glm::vec3(2 * u, 1.5 * u, 0) - glm::vec3(0, 4 * u, 0), color1),
		VertexFormat(middle + glm::vec3(2 * u, 0.5 * u, 0) - glm::vec3(0, 4 * u, 0), color1),
		VertexFormat(middle + glm::vec3(0, 2.5 * u, 0) - glm::vec3(0, 4 * u, 0), color1),
		VertexFormat(middle + glm::vec3(-2 * u, 0.5 * u, 0) - glm::vec3(0, 4 * u, 0), color1),
		VertexFormat(middle + glm::vec3(-2 * u, 1.5 * u, 0) - glm::vec3(0, 4 * u, 0), color1)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 4, 5, 6, 4, 6, 7, 4, 7, 8, 4, 8, 9, 10, 11, 12, 10, 12, 13, 10, 13, 14, 10, 14, 15, 16, 17, 18, 16, 18, 19, 16, 19, 20, 16, 20, 21, 0, 1, 2, 0, 2, 3, };
	
	if (!fill) {
		square->SetDrawMode(GL_TRIANGLES);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

//Create the shield powerup
Mesh* Powerup::CreateShield(std::string name, glm::vec3 middle, float height, float width, bool fill)
{
	float u = height / 11;
	glm::vec3 color = glm::vec3(0.15, 0.64, 0.28);
	glm::vec3 color1 = glm::vec3(1, 1, 1);
	glm::vec3 color2 = glm::vec3(0.77, 0.77, 0.77);
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(middle - glm::vec3(width / 2, height / 2, 0), color),
		VertexFormat(middle + glm::vec3(width, 0, 0) - glm::vec3(width / 2, height / 2, 0), color),
		VertexFormat(middle + glm::vec3(width, height, 0) - glm::vec3(width / 2, height / 2, 0), color),
		VertexFormat(middle + glm::vec3(0, height, 0) - glm::vec3(width / 2, height / 2, 0), color),
		VertexFormat(middle + glm::vec3(0, 4.5 * u, 0), color1),
		VertexFormat(middle + glm::vec3(4 * u, 2.5 * u, 0), color1),
		VertexFormat(middle + glm::vec3(3 * u, -2.5 * u, 0), color1),
		VertexFormat(middle + glm::vec3(0, -4.5 * u, 0), color1),
		VertexFormat(middle + glm::vec3(0, -4.5 * u, 0), color2),
		VertexFormat(middle + glm::vec3(-3 * u, -2.5 * u, 0), color2),
		VertexFormat(middle + glm::vec3(-4 * u, 2.5 * u, 0), color2),
		VertexFormat(middle + glm::vec3(0, 4.5 * u, 0), color2),
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 4, 5, 6, 4, 6, 7, 11, 8, 9, 11, 9, 10, 0, 1, 2, 0, 2, 3, };

	if (!fill) {
		square->SetDrawMode(GL_TRIANGLES);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}
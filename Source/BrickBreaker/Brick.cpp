#include "Brick.h"

#include <Core/Engine.h>

using namespace std;

Brick::Brick(float x, float y, float height, float width) {
	this->x = x;
	this->y = y;
	this->height = height;
	this->width = width;
	this->visible = 1;
	this->scaleX = 1.f;
	this->scaleY = 1.f;
}

void Brick::setScale(float scaleX, float scaleY) {
	this->scaleX = scaleX;
	this->scaleY = scaleY;
}

glm::vec2 Brick::getScale() {
	return glm::vec2(this->x, this->y);
}

int Brick::getVisible() {
	return this->visible;
}

void Brick::setVisible() {
	this->visible = 0;
}

void Brick::resetVisible() {
	this->visible = 1;
}

Brick::~Brick() {

}

glm::vec4 Brick::getBorders() {
	return glm::vec4(this->y - this->height / 2, this->y + this->height / 2, this->x - this->width / 2, this->x + this->width / 2);
}

Mesh* Brick::CreateBrick(std::string name, glm::vec3 middle, float height, float width, glm::vec3 color, bool fill)
{

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(middle - glm::vec3(width / 2, height / 2, 0), color),
		VertexFormat(middle + glm::vec3(width, 0, 0) - glm::vec3(width / 2, height / 2, 0), color),
		VertexFormat(middle + glm::vec3(width, height, 0) - glm::vec3(width / 2, height / 2, 0), color),
		VertexFormat(middle + glm::vec3(0, height, 0) - glm::vec3(width / 2, height / 2, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };
	
	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}
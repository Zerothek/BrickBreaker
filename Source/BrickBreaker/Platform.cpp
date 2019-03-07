#include "Platform.h"

#include <Core/Engine.h>

using namespace std;

Platform::Platform(float x, float y, float width) {
	this->x = x;
	this->y = y;
	this->width = width;
}

Platform::~Platform() {

}

void Platform::changeMiddle(float x) {
	this->x = x;
	//used to change the Platform position(only on the x axis)
}

float Platform::rightBorder() {
	return this->x + width / 2;
	/*get the right border of the Platform.Usefull for the collission with the
	edge of the screen*/
}

float Platform::leftBorder() {
	return this->x - width / 2;
	/*get the left border of the Platform.Usefull for the collission with the
	edge of the screen*/
}

Mesh* Platform::CreatePlatform(std::string name, glm::vec3 middle, float height, float width, glm::vec3 color, bool fill)
{

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(middle - glm::vec3(width / 2, height / 2, 0) + glm::vec3(width / 4, 0, 0), color),
		VertexFormat(middle + glm::vec3(width, 0, 0) - glm::vec3(width / 2, height / 2, 0) - glm::vec3(width / 4, 0, 0), color),
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
#include "Heart.h"

#include <Core/Engine.h>

using namespace std;

Heart::Heart() {
}

Heart::~Heart() {
}

Mesh* Heart::CreateHeart(std::string name, glm::vec3 tip, float unit, glm::vec3 color, bool fill)
{

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(tip, color),
		VertexFormat(tip + glm::vec3(2 * unit, unit * 3 / 2, 0), color),
		VertexFormat(tip + glm::vec3(3 * unit, 3 * unit, 0), color),
		VertexFormat(tip + glm::vec3(3 * unit, 4 * unit, 0), color),
		VertexFormat(tip + glm::vec3(2 * unit, 5 * unit, 0), color),
		VertexFormat(tip + glm::vec3(unit, 5 * unit, 0), color),
		VertexFormat(tip + glm::vec3(0, 4 * unit, 0), color),
		VertexFormat(tip + glm::vec3(-unit, 5 * unit, 0), color),
		VertexFormat(tip + glm::vec3(-2 * unit, 5 * unit, 0), color),
		VertexFormat(tip + glm::vec3(-3 * unit, 4 * unit, 0), color),
		VertexFormat(tip + glm::vec3(-3 * unit, 3 * unit, 0), color),
		VertexFormat(tip + glm::vec3(-2 * unit, unit * 3 / 2, 0), color),
	};

	Mesh* heart = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 0, 2, 3 , 0, 3, 4, 0, 4, 5, 0, 5, 6, 0, 6, 7, 0, 7, 8, 0, 8, 9, 0, 9, 10, 0, 10, 11};
	
	if (!fill) {
		heart->SetDrawMode(GL_TRIANGLES);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	heart->InitFromData(vertices, indices);
	return heart;
}
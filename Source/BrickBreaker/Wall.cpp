#include "Wall.h"

#include <Core/Engine.h>

Mesh* Wall::CreateWall(std::string name, glm::vec3 middle, float height, float width, glm::vec3 color, bool fill)
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
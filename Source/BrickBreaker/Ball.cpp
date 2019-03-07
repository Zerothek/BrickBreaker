#include "Ball.h"

#include <Core/Engine.h>

using namespace std;

Ball::Ball(float x, float y, float radius) {
	this->x = x;
	this->y = y;
	this->radius = radius;
}

Ball::~Ball() {

}


Mesh* Ball::CreateBall(std::string name, glm::vec3 middle, float radius, glm::vec3 color, bool fill)
{

	std::vector<VertexFormat> vertices;
	vertices.push_back(middle);
	for (int i = 0; i < 720; i++) {
		double u = 2 * M_PI * i / 720;
		VertexFormat point = VertexFormat(middle + glm::vec3(sin(u) * radius, cos(u) * radius, 0), color);
		vertices.push_back(point);
	}

	Mesh* circle = new Mesh(name);
	std::vector<unsigned short> indices;
	for (int i = 0; i < 720; i++) {
		indices.push_back(i);
	}
	
		circle->SetDrawMode(GL_TRIANGLE_FAN);

	circle->InitFromData(vertices, indices);
	return circle;
}
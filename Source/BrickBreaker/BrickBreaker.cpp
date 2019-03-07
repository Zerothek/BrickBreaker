#include "BrickBreaker.h"

#include <vector>
#include <iostream>
#include <string>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Brick.h"
#include "Wall.h"
#include "Platform.h"
#include "Ball.h"
#include "Heart.h"
#include "Powerup.h"

using namespace std;

BrickBreaker::BrickBreaker()
{
}

BrickBreaker::~BrickBreaker()
{
}


int wall_height = 720, wall_width = 10;
//Dimensions for a wall
float platform_x = -1, platform_width = 200, platform_height = 15, base_platform_width = 200;
//Initial values for the platform
int spacer = 15, wall_spacer = 50;
//spacer = the distance between bricks, wall_spacer = the distance between the wall and the first brick
float brick_width = (1280 - 2 * wall_width - 2 * wall_spacer - 11 * spacer) / 12, brick_height = brick_width / 2;
//the dimensions of the bricks such that on a 1280p screen 12 bricks will be on a row
float ball_radius = 12, ball_x, ball_y, ball_cos = 0, ball_speed = 300, base_ball_speed = 300;
//Initial values for the ball
int ball_click = 0, ball_horizontal = 1, ball_vertical = 1;
//ball click is used to detect if the ball has been launched(by a left mouse click)
/*ball_horizontal and ball_vertical are used to mirror the trajectory of the ball on the x or
the y axis.Example:if the ball is hiting the bottom of a brick then ball_vertical will flip to -1,
because the ball needs to travel in the opposite way on the y axis but on the same way on the x axis*/ 
Platform *platform;
Brick *bricks[60]; //a vector of Bricks
Brick *leftWall, *rightWall, *topWall, *bottomWall; //the 4 red walls
float scales[120]; //the scale on the x and y axis for a brick.it will be used for the disapearing brick animation
Heart *hearts[3];
int num_hearts = 3;
Powerup *powerup[5];
int powerup_height = 30;
int powerup_width = 30;
int powerup_x = -100, powerup_y = -100;
float rotation = 0; //used to rotate the powerups
float scale_platform = 0;
int wrecked_bricks;
int bottom_visible = 0; //used to check if the bottom wall is visible(shield PowerUp) 
float shield_time, big_time; //used to countdown the Shield PowerUp time

void BrickBreaker::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 middle = glm::vec3(0, 0, 0);
	

	shield_time = big_time = 0;
	
	//Create the 60 brick Objects
	for (int i = 0; i < 60; i++) {
		bricks[i] = new Brick(wall_width + wall_spacer + brick_width / 2 + (spacer + brick_width) * (i % 12), 720 - brick_height / 2 - wall_width - spacer - (spacer + brick_height) * (i / 12), brick_height, brick_width);
		string mesh = "brick";
		mesh.append(std::to_string(i / 12));
		mesh.append(std::to_string(i % 12));
		Mesh *brick1 = bricks[i]->CreateBrick(mesh, middle, brick_height, brick_width, glm::vec3(1, 0, 0), true);
		AddMeshToList(brick1);
		scales[2 * i] = 1;
		scales[2 * i + 1] = 1;
	}

	//Create the left, right, bottom and top walls
	leftWall = new Brick(wall_width / 2, wall_height / 2, wall_height, wall_width);
	Mesh* left_wall = Wall::CreateWall("left_wall", middle, wall_height, wall_width, glm::vec3(1, 0, 0), true);
	AddMeshToList(left_wall);

	rightWall = new Brick(1280 - wall_width / 2, wall_height / 2, wall_height, wall_width);
	Mesh* right_wall = Wall::CreateWall("right_wall", middle, wall_height, wall_width, glm::vec3(1, 0, 0), true);
	AddMeshToList(right_wall);

	topWall = new Brick(1280 / 2, 720 - wall_width / 2, wall_width, 1280);
	Mesh* top_wall = Wall::CreateWall("top_wall", middle, wall_width, 1280, glm::vec3(1, 0, 0), true);
	AddMeshToList(top_wall);

	bottomWall = new Brick(1280 / 2, wall_width / 2, wall_width, 1280);
	Mesh* bottom_wall = Wall::CreateWall("bottom_wall", middle, wall_width, 1280, glm::vec3(1, 0, 0), true);
	AddMeshToList(bottom_wall);


	//Create the {latform Object
	platform = new Platform(640, 100, platform_width);
	Mesh* platform1 = platform->CreatePlatform("platform", middle, platform_height, platform_width, glm::vec3(1, 1, 1), true);
	AddMeshToList(platform1);

	//Create the ball object
	Ball *ball = new Ball(50, 50, 10);
	Mesh* ball1 = ball->CreateBall("ball", middle, ball_radius, glm::vec3(1, 1, 1), true);
	AddMeshToList(ball1);

	//Create the 3 hearts object
	for (int i = 0; i < 3; i++) {
		hearts[i] = new Heart();
		string mesh = "heart";
		mesh.append(std::to_string(i));
		Mesh *heart = hearts[i]->CreateHeart(mesh, middle, 4, glm::vec3(1, 0, 0), true);
		AddMeshToList(heart);
	}

	//Create the 5 PowerUps Objects
	for (int i = 0; i < 5; i++) {
		powerup[i] = new Powerup(0, 0, powerup_height, powerup_width);
		string mesh = "powerup";
		mesh.append(std::to_string(i));
		if (i > 2) {
			Mesh *pwr = powerup[i]->CreateShield(mesh, middle, powerup_height, powerup_width, true);
			AddMeshToList(pwr);
		}
		else {
			Mesh *pwr = powerup[i]->CreatePowerup(mesh, middle, powerup_height, powerup_width, true);
			AddMeshToList(pwr);
		}
		
	}
}

void BrickBreaker::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void BrickBreaker::Update(float deltaTimeSeconds)
{
	//If the player loses a life or the game is one the game resets accordingly
	if (ball_x < 0 || ball_x > 1280 || ball_y < 0 || ball_y > 720 || wrecked_bricks == 60) {
		ball_click = 0;
		ball_cos = 0;
		ball_vertical = 1;
		ball_horizontal = 1;
		num_hearts--;
		ball_speed = base_ball_speed;
		shield_time = big_time = 0;
		if (num_hearts == 0) {
			bottom_visible = 0;
			wrecked_bricks = 0;
			num_hearts = 3;
			for (int i = 0; i < 60; i++) {
				scales[2 * i] = 1;
				scales[2 * i + 1] = 1;
				bricks[i]->resetVisible();
			}
		}
	}

	//decrease the shield_time if the Shield Powerup is active
	if (shield_time > 0) {
		shield_time -= deltaTimeSeconds;
	}

	//decrease the big_time if the Get Big Powerup is active
	if (big_time > 0) {
		big_time -= deltaTimeSeconds;
	}

	//if the time expires the powerup will expire
	if (shield_time <= 0) {
		bottom_visible = 0;
		ball_speed = base_ball_speed;
	}

	if (big_time <= 0) {
		scale_platform = 0;
		platform->width = base_platform_width;
		platform_width = platform->width;
	}

	/*iterate through the brick vector.If a brick has been destroyed continue.Else if a brick
	has been hit but not yet destroyed scale it down by a factor depending on deltatime in order to
	obtain an animation*/
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 12; j++) {
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(wall_width + wall_spacer + brick_width / 2 + (spacer + brick_width) * j, 720 - brick_height / 2 - wall_width - spacer - (spacer + brick_height) * i); // width / 2 + 25, 720 - height / 2 - 25
			if (bricks[i * 12 + j]->getVisible() == 0) {
				//continue;
				glm::vec2 scale = bricks[i]->getScale();
				float scX = scales[2 * (i * 12 + j)], scY = scales[2 * (i * 12 + j) + 1];
				if (scX >= 0 && scY >= 0) {
					modelMatrix *= Transform2D::Scale(scX, scY);
					bricks[i]->setScale(scX, scY);
					scX -= deltaTimeSeconds;
					scY -= deltaTimeSeconds;
					scales[2 * (i * 12 + j)] = scX;
					scales[2 * (i * 12 + j) + 1] = scY;
				}
				else {
					modelMatrix *= Transform2D::Scale(0, 0);
				}
			}
			string mesh = "brick";
			mesh.append(std::to_string(i));
			mesh.append(std::to_string(j));
			RenderMesh2D(meshes[mesh], shaders["VertexColor"], modelMatrix);
		}
	}

	//Draw the "pulsating" hearts
	for (int i = 0; i < num_hearts; i++) {
		string mesh = "heart";
		mesh.append(std::to_string(i));

		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(40 + i * 30, 30);
		modelMatrix *= Transform2D::Scale(hearts[i]->scale, hearts[i]->scale);
		modelMatrix *= Transform2D::Translate(0, -10);
		if (hearts[i]->scaleDown == true) {
			hearts[i]->scale -= deltaTimeSeconds / 2;
		}
		else {
			hearts[i]->scale += deltaTimeSeconds / 2;
		}
		if (hearts[i]->scale >= 1) {
			hearts[i]->scaleDown = true;
		}
		if (hearts[i]->scale <= 0.75) {
			hearts[i]->scaleDown = false;
		}

		RenderMesh2D(meshes[mesh], shaders["VertexColor"], modelMatrix);
	}


	//Draw the walls

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(wall_width / 2, wall_height / 2);
	RenderMesh2D(meshes["left_wall"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(1280 - wall_width / 2, wall_height / 2);
	RenderMesh2D(meshes["right_wall"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(1280 / 2, 720 - wall_width / 2);
	RenderMesh2D(meshes["top_wall"], shaders["VertexColor"], modelMatrix);

	if (bottom_visible == 1) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(1280 / 2, wall_width / 2);
		RenderMesh2D(meshes["bottom_wall"], shaders["VertexColor"], modelMatrix);
	}


	/*Adjust the platform position considering the collission with the walls and 
	considering if the Get Big PowerUp is activated*/
	modelMatrix = glm::mat3(1);
	platform->changeMiddle(platform_x);
	if (platform->rightBorder() >= 1280 - wall_width) {
		if (scale_platform == 0) {
			modelMatrix *= Transform2D::Translate(1280 - wall_width - platform_width / 2, 60); 
			RenderMesh2D(meshes["platform"], shaders["VertexColor"], modelMatrix);
		} else {
			modelMatrix *= Transform2D::Translate(1280 - wall_width - platform_width / 2, 60); 
			modelMatrix *= Transform2D::Scale(scale_platform, 1);
			RenderMesh2D(meshes["platform"], shaders["VertexColor"], modelMatrix);
		}
		if (ball_click == 0) {
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(1280 - wall_width - platform_width / 2, 5 + 60 + platform_height / 2 + ball_radius);
			ball_x = 1280 - wall_width - platform_width / 2;
			ball_y = 5 + 60 + platform_height / 2 + ball_radius;
			RenderMesh2D(meshes["ball"], shaders["VertexColor"], modelMatrix);
		}
	}
	else if (platform->leftBorder() < wall_width) {
		if (scale_platform == 0) {
			modelMatrix *= Transform2D::Translate(platform_width / 2 + wall_width, 60); //Platform
			RenderMesh2D(meshes["platform"], shaders["VertexColor"], modelMatrix);
		} else {
			modelMatrix *= Transform2D::Translate(platform_width / 2 + wall_width, 60); //Platform
			modelMatrix *= Transform2D::Scale(scale_platform, 1);
			RenderMesh2D(meshes["platform"], shaders["VertexColor"], modelMatrix);
		}
		if (ball_click == 0) {
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(platform_width / 2 + wall_width, 5 + 60 + platform_height / 2 + ball_radius);
			ball_x = platform_width / 2 + wall_width;
			ball_y = 5 + 60 + platform_height / 2 + ball_radius;
			RenderMesh2D(meshes["ball"], shaders["VertexColor"], modelMatrix);
		}
	}
	else {
		if (scale_platform == 0) {
			modelMatrix *= Transform2D::Translate(platform_x, 60); //Platform
			RenderMesh2D(meshes["platform"], shaders["VertexColor"], modelMatrix);
		} else {
			modelMatrix *= Transform2D::Translate(platform_x, 60); //Platform
			modelMatrix *= Transform2D::Scale(scale_platform, 1);
			RenderMesh2D(meshes["platform"], shaders["VertexColor"], modelMatrix);
		}
		if (ball_click == 0) {
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(platform_x, 5 + 60 + platform_height / 2 + ball_radius);
			ball_x = platform_x;
			ball_y = 5 + 60 + platform_height / 2 + ball_radius;
			RenderMesh2D(meshes["ball"], shaders["VertexColor"], modelMatrix);
		}
	}

	//adjust the ball position according to the trajectory of the ball and the speed of the ball
	if (ball_click == 1 && ball_vertical == 1) {
		ball_y -= (asin(ball_cos) - M_PI / 2) * ball_speed * deltaTimeSeconds;
	}

	if (ball_click == 1 && ball_vertical == -1) {
		ball_y += (asin(ball_cos) - M_PI / 2) * ball_speed * deltaTimeSeconds;
	}

	if (ball_click == 1 && ball_horizontal == 1) {
		ball_x += (acos(ball_cos) - M_PI / 2) * ball_speed * deltaTimeSeconds;
	}

	if (ball_click == 1 && ball_horizontal == -1) {
		ball_x -= (acos(ball_cos) - M_PI / 2) * ball_speed * deltaTimeSeconds;
	}

	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(ball_x, ball_y);
	RenderMesh2D(meshes["ball"], shaders["VertexColor"], modelMatrix);



	//Powerups
	//Check if we hit a brick that drops a powerup and begin a droping animation if so.
	rotation += 2.5 * deltaTimeSeconds;
	for (int i = 0; i < 5; i++) {
		if (powerup[i]->getVisible() == 1) {
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(powerup[i]->x, powerup[i]->y);
			modelMatrix *= Transform2D::Rotate(rotation);
			string mesh = "powerup";
			mesh.append(std::to_string(i));
			RenderMesh2D(meshes[mesh], shaders["VertexColor"], modelMatrix);
		}
		powerup[i]->y -= deltaTimeSeconds * 150;
		int error = 1;
		if (powerup[i]->y <= 60 + platform_height / 2 && powerup[i]->y >= 60 + platform_height / 2 - 5 &&
			powerup[i]->x >= platform_x - platform_width / 2 && powerup[i]->x <= platform_x + platform_width / 2) {
			powerup[i]->resetVisible();
			if (i <= 2) {
				if (platform_width == base_platform_width) {
					scale_platform = 1.5;
					platform->width *= 1.5;
					platform_width = platform->width;
					big_time = 20;
				}
			}
			else {
				if (ball_speed < 2 * base_ball_speed) {
					ball_speed *= 1.5;
				}
				bottom_visible = 1;
				shield_time = 20;
			}
		}
	}


	//Bricks Collision
	/*Check if we hit a brick(on the top, bottom, left or right side) in order to adjust
	the ball_vertical and ball_horizontal variables*/
	for (int i = 0; i < 60; i++) {
		if (bricks[i]->getVisible() == 0) {
			continue;
		}
		glm::vec4 borders = bricks[i]->getBorders(); // down, up, left, right
		float down = borders[0], up = borders[1], left = borders[2], right = borders[3], error = 1;
		for (double j = 0; j < 2 * M_PI; j = j + (M_PI / 180)) {

			//collission with bottom of brick
			if (ball_x + cos(j) * ball_radius >= left && ball_x + cos(j) * ball_radius <= right) {
				if (ball_y + sin(j) * ball_radius >= down && ball_y + sin(j) * ball_radius <= down + error) {
					wrecked_bricks++;
					ball_vertical *= -1;
					bricks[i]->setVisible();
					if (i / 12 == 4 && i % 12 == 5) {
						powerup[0]->setCoordinates((right - left) / 2 + left, (up - down) / 2 + down);
						powerup[0]->setVisible();
					}
					if (i / 12 == 2 && i % 12 == 2) {
						powerup[1]->setCoordinates((right - left) / 2 + left, (up - down) / 2 + down);
						powerup[1]->setVisible();
					}
					if (i / 12 == 1 && i % 12 == 6) {
						powerup[2]->setCoordinates((right - left) / 2 + left, (up - down) / 2 + down);
						powerup[2]->setVisible();
					}
					if (i / 12 == 4 && i % 12 == 9) {
						powerup[3]->setCoordinates((right - left) / 2 + left, (up - down) / 2 + down);
						powerup[3]->setVisible();
					}
					if (i / 12 == 3 && i % 12 == 7) {
						powerup[4]->setCoordinates((right - left) / 2 + left, (up - down) / 2 + down);
						powerup[4]->setVisible();
					}
					break;
				}
			}
		}

			for (double j = 0; j < 2 * M_PI; j = j + (M_PI / 180)) {
			//collission with top of brick
			if (ball_x + cos(j) * ball_radius >= left && ball_x + cos(j) * ball_radius <= right) {
				if (ball_y + sin(j) * ball_radius >= up - error && ball_y + sin(j) * ball_radius <= up) {
					wrecked_bricks++;
					ball_vertical *= -1;
					bricks[i]->setVisible();
					if (i / 12 == 4 && i % 12 == 5) {
						powerup[0]->setCoordinates((right - left) / 2 + left, (up - down) / 2 + down);
						powerup[0]->setVisible();
					}
					if (i / 12 == 2 && i % 12 == 2) {
						powerup[1]->setCoordinates((right - left) / 2 + left, (up - down) / 2 + down);
						powerup[1]->setVisible();
					}
					if (i / 12 == 1 && i % 12 == 6) {
						powerup[2]->setCoordinates((right - left) / 2 + left, (up - down) / 2 + down);
						powerup[2]->setVisible();
					}
					if (i / 12 == 4 && i % 12 == 9) {
						powerup[3]->setCoordinates((right - left) / 2 + left, (up - down) / 2 + down);
						powerup[3]->setVisible();
					}
					if (i / 12 == 3 && i % 12 == 7) {
						powerup[4]->setCoordinates((right - left) / 2 + left, (up - down) / 2 + down);
						powerup[4]->setVisible();
					}
					break;
				}
			}
		}

			for (double j = 0; j < 2 * M_PI; j = j + (M_PI / 180)) {
			//collission with left side of brick
			if (ball_y + sin(j) * ball_radius >= down && ball_y + sin(j) * ball_radius <= up) {
				if (ball_x + cos(j) * ball_radius >= left && ball_x + cos(j) * ball_radius <= left + error) {
					wrecked_bricks++;
					ball_horizontal *= -1;
					bricks[i]->setVisible();
					if (i / 12 == 4 && i % 12 == 5) {
						powerup[0]->setCoordinates((right - left) / 2 + left, (up - down) / 2 + down);
						powerup[0]->setVisible();
					}
					if (i / 12 == 2 && i % 12 == 2) {
						powerup[1]->setCoordinates((right - left) / 2 + left, (up - down) / 2 + down);
						powerup[1]->setVisible();
					}
					if (i / 12 == 1 && i % 12 == 6) {
						powerup[2]->setCoordinates((right - left) / 2 + left, (up - down) / 2 + down);
						powerup[2]->setVisible();
					}
					if (i / 12 == 4 && i % 12 == 9) {
						powerup[3]->setCoordinates((right - left) / 2 + left, (up - down) / 2 + down);
						powerup[3]->setVisible();
					}
					if (i / 12 == 3 && i % 12 == 7) {
						powerup[4]->setCoordinates((right - left) / 2 + left, (up - down) / 2 + down);
						powerup[4]->setVisible();
					}
					break;
				}
			}
		}

			//collission with right side of brick
			for (double j = 0; j < 2 * M_PI; j = j + (M_PI / 180)) {
			if (ball_y + sin(j) * ball_radius >= down && ball_y + sin(j) * ball_radius <= up) {
				if (ball_x + cos(j) * ball_radius >= right - error && ball_x + cos(j) * ball_radius <= right) {
					wrecked_bricks++;
					ball_horizontal *= -1;
					bricks[i]->setVisible();

					if (i / 12 == 4 && i % 12 == 5) {
						powerup[0]->setCoordinates((right - left) / 2 + left, (up - down) / 2 + down);
						powerup[0]->setVisible();
					}
					if (i / 12 == 2 && i % 12 == 2) {
						powerup[1]->setCoordinates((right - left) / 2 + left, (up - down) / 2 + down);
						powerup[1]->setVisible();
					}
					if (i / 12 == 1 && i % 12 == 6) {
						powerup[2]->setCoordinates((right - left) / 2 + left, (up - down) / 2 + down);
						powerup[2]->setVisible();
					}
					if (i / 12 == 4 && i % 12 == 9) {
						powerup[3]->setCoordinates((right - left) / 2 + left, (up - down) / 2 + down);
						powerup[3]->setVisible();
					}
					if (i / 12 == 3 && i % 12 == 7) {
						powerup[4]->setCoordinates((right - left) / 2 + left, (up - down) / 2 + down);
						powerup[4]->setVisible();
					}
					break;
				}
			}
		}
	}

	/*Check if we hit a wall(top, bottom, left or right wall) in order to adjust
	the ball_vertical and ball_horizontal variables*/
	for (double j = 0; j < 2 * M_PI; j = j + (M_PI / 360)) {
		int error = 1;
		//Collission left side wall
		if (ball_y + sin(j) * ball_radius >= 0 && ball_y + sin(j) * ball_radius <= 720) {
			if (ball_x + cos(j) * ball_radius >= wall_width - error && ball_x + cos(j) * ball_radius <= wall_width) {
				ball_horizontal = -1;
				break;
			}
		}

		//Collission right side wall
		if (ball_y + sin(j) * ball_radius >= 0 && ball_y + sin(j) * ball_radius <= 720) {
			if (ball_x + cos(j) * ball_radius >= 1280 - wall_width && ball_x + cos(j) * ball_radius <= 1280 - wall_width + error) {
				ball_horizontal = 1;
				break;
			}
		}

		//collission with top Wall
		if (ball_x + cos(j) * ball_radius >= 0 && ball_x + cos(j) * ball_radius <= 1280) {
			if (ball_y + sin(j) * ball_radius >= 720 - wall_width && 
				ball_y + sin(j) * ball_radius <= 720 - wall_width + error) {
				ball_vertical = -1;
				break;
			}
		}

		//collission with bottom Wall if activated
		if (bottom_visible == 1) {
			if (ball_x + cos(j) * ball_radius >= 0 && ball_x + cos(j) * ball_radius <= 1280) {
				if (ball_y + sin(j) * ball_radius <= wall_width &&
					ball_y + sin(j) * ball_radius >= wall_width - error) {
					ball_vertical = 1;
					ball_speed /= 2;
					bottom_visible = 0;
					break;
				}
			}
		}
	}



	//Platform collision
	/*if the ball and the platform collided reset the ball_vertical and ball_horizontal variables
	to 1 and calculate a new angle(trajectory) for the ball depending on which point the two collided
	relative to the center of the platform8*/
	for (double j = 0; j < 2 * M_PI; j = j + (M_PI / 360)) {
		if (ball_x + cos(j) * ball_radius >= platform_x - platform_width / 2 && 
			ball_x + cos(j) * ball_radius <= platform_x + platform_width / 2 && 
			ball_y + sin(j) * ball_radius <= 60 + platform_height / 2 && ball_click != 0 && 
			ball_y + sin(j) * ball_radius >= 60 - platform_height / 2) {
			ball_vertical = 1;
			ball_cos = (platform_x - ball_x) / (platform_width / 2);
			if (ball_cos > 0) {
				ball_horizontal = 1;
			}
			else {
				ball_horizontal = -1;
				ball_cos *= -1;
			}
			break;
		}
	}

}

void BrickBreaker::FrameEnd()
{

}

void BrickBreaker::OnInputUpdate(float deltaTime, int mods)
{
	
}

void BrickBreaker::OnKeyPress(int key, int mods)
{
	// add key press event
}

void BrickBreaker::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void BrickBreaker::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
	platform_x = mouseX;
}

void BrickBreaker::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
	ball_click = 1;
}

void BrickBreaker::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void BrickBreaker::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void BrickBreaker::OnWindowResize(int width, int height)
{
}

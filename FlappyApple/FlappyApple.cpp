#include <iostream>
#include <deque>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader_s.h"
#include "Background.h"
#include "Apple.h"
#include "Wall.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void applyVMovement(glm::mat4 &vMovement, int vMovementLoc, glm::mat4 &aRotate, int aRotateLoc);
void applyHMovement(glm::mat4 hMovement, int hMovementLoc, Wall &wall, bool game_active);
void applyGravity();
bool checkCollision(Apple ap, glm::mat4 vMovement, glm::mat4 aRotate, Wall wall);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float vVelocity = 0.0f;
float vPosition = SCR_HEIGHT / 2;
const float H_VELOCITY = 200.0f;
const float GRAVITY = -600.0f;
const float MAX_V_VELOCITY = 300.0f;
const float MIN_V_VELOCITY = -300.0f;

bool game_active = true;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Flappy Apple", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// shader class here
	Shader ourShader("shader.vs", "shader.fs");

	Background bg;
	Apple ap;
	std::deque<Wall> walls;
	walls.push_back(Wall());

	ourShader.use();

	glm::mat4 vMovement = glm::mat4(1.0f);
	int vMovementLoc = glGetUniformLocation(ourShader.ID, "vMovement");
	

	glm::mat4 hMovement = glm::mat4(1.0f);
	int hMovementLoc = glGetUniformLocation(ourShader.ID, "hMovement");

	glm::mat4 aRotate = glm::mat4(1.0f);
	int aRotateLoc = glGetUniformLocation(ourShader.ID, "aRotate");
	
	
	glm::mat4 projection;
	projection = glm::ortho(0.0f, float(SCR_WIDTH), float(SCR_HEIGHT), 0.0f, -1.0f, 1.0f);
	int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		glfwSetKeyCallback(window, keyCallback);

		glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// activate shader
		ourShader.use();

		// texture units

		// matrices
		if (game_active) {
			applyVMovement(vMovement, vMovementLoc, aRotate, aRotateLoc);
			applyGravity();
		}
		

		glUniform1i(glGetUniformLocation(ourShader.ID, "obj"), 0);
		glBindVertexArray(bg.getVAO());
		glDrawArrays(GL_TRIANGLES, 0, 6); // draw elements

		glUniform1i(glGetUniformLocation(ourShader.ID, "obj"), 1);
		if (game_active) {
			for (int i = 0; i < walls.size(); i++) {
				applyHMovement(hMovement, hMovementLoc, walls[i], game_active);
			
				glBindVertexArray(walls[i].getVAO());
				glDrawArrays(GL_TRIANGLES, 0, 12); // draw elements
				if (walls[i].hPosition > 600.0f && walls.size() == i + 1) {
					walls.push_back(Wall());
				}
			}
			if (walls[0].hPosition > 1375.0f) {
				walls[0].deleteObjects();
				walls.pop_front();
			}
		}
		else {
			for (int i = 0; i < walls.size(); i++) {
				applyHMovement(hMovement, hMovementLoc, walls[i], game_active);

				glBindVertexArray(walls[i].getVAO());
				glDrawArrays(GL_TRIANGLES, 0, 12); // draw elements
			}
		}
		
		

		glUniform1i(glGetUniformLocation(ourShader.ID, "obj"), 2);
		glBindVertexArray(ap.getVAO());
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		for (int i = 0; i < walls.size(); i++) {
			if (1300.0f - walls[i].hPosition >= 585.0f - 75.0f && 1375.0f - walls[i].hPosition <= 695.0f + 75.0f) {
				if (checkCollision(ap, vMovement, aRotate, walls[i])) {
					game_active = false;
					break;
				}
			}
		}

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	bg.deleteObjects();
	ap.deleteObjects();

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		vVelocity += -GRAVITY;
		if (vVelocity > MAX_V_VELOCITY) {
			vVelocity = MAX_V_VELOCITY;
		}
	}
}

void applyVMovement(glm::mat4 &vMovement, int vMovementLoc, glm::mat4 &aRotate, int aRotateLoc) {
	vPosition += vVelocity * deltaTime;
	if (vPosition < 32.0f) {
		vPosition = 32.0f;
	}
	else if (vPosition > 720.0f - 32.0f) {
		vPosition = 720.0f - 32.0f;
	}

	vMovement = glm::mat4(1.0f);
	vMovement = glm::translate(vMovement, glm::vec3(0.0f, -vPosition, 0.0f));
	glUniformMatrix4fv(vMovementLoc, 1, GL_FALSE, glm::value_ptr(vMovement));

	float rad = vVelocity / MAX_V_VELOCITY * 45.0f;
	aRotate = glm::mat4(1.0f);
	aRotate = glm::translate(aRotate, glm::vec3(640.0f, 720.0f, 0.0f));
	aRotate = glm::rotate(aRotate, glm::radians(-rad), glm::vec3(0.0f, 0.0f, 1.0f));
	aRotate = glm::translate(aRotate, glm::vec3(-640.0f, -720.0f, 0.0f));
	glUniformMatrix4fv(aRotateLoc, 1, GL_FALSE, glm::value_ptr(aRotate));
}

void applyHMovement(glm::mat4 hMovement, int hMovementLoc, Wall &wall, bool game_active) {
	if (game_active) {
		wall.hPosition += H_VELOCITY * deltaTime;
	}
	

	hMovement = glm::translate(hMovement, glm::vec3(-wall.hPosition, 0.0f, 0.0f));
	glUniformMatrix4fv(hMovementLoc, 1, GL_FALSE, glm::value_ptr(hMovement));
}

void applyGravity() {
	if (vVelocity > MIN_V_VELOCITY) {
		vVelocity += GRAVITY * deltaTime;
	}
	else {
		vVelocity = MIN_V_VELOCITY;
	}
}

bool checkCollision(Apple ap, glm::mat4 vMovement, glm::mat4 aRotate, Wall wall) {
	glm::vec4 ap_bottom_left = glm::vec4(ap.vertices[0], ap.vertices[1], 0.0f, 1.0f);
	glm::vec4 ap_top_left = glm::vec4(ap.vertices[6], ap.vertices[7], 0.0f, 1.0f);
	glm::vec4 ap_bottom_right = glm::vec4(ap.vertices[12], ap.vertices[13], 0.0f, 1.0f);
	glm::vec4 ap_top_right = glm::vec4(ap.vertices[18], ap.vertices[19], 0.0f, 1.0f);
	ap_bottom_left = vMovement * aRotate * ap_bottom_left;
	ap_top_left = vMovement * aRotate * ap_top_left;
	ap_bottom_right = vMovement * aRotate * ap_bottom_right;
	ap_top_right = vMovement * aRotate * ap_top_right;
	glm::vec2 top_wall_bottom_left = glm::vec2(1300.0f - wall.hPosition, wall.minY + wall.height + wall.rand);
	glm::vec2 top_wall_bottom_right = glm::vec2(1375.0f - wall.hPosition, wall.minY + wall.height + wall.rand);
	glm::vec2 bottom_wall_top_left = glm::vec2(1300.0f - wall.hPosition, wall.maxY - wall.height + wall.rand);
	glm::vec2 bottom_wall_top_right = glm::vec2(1375.0f - wall.hPosition, wall.maxY - wall.height + wall.rand);

	// check if all y coordinates are in the bounds of the open space
	if (ap_top_left.y > top_wall_bottom_left.y && ap_top_right.y > top_wall_bottom_left.y 
		&& ap_bottom_left.y < bottom_wall_top_left.y && ap_bottom_right.y < bottom_wall_top_left.y) {
		return false;
	}
	else {
		// check if all coordinate x values are outside the bounds of the open space
		if ((ap_top_left.x > top_wall_bottom_right.x || ap_top_right.x < top_wall_bottom_left.x) 
			&& (ap_bottom_left.x > bottom_wall_top_right.x || ap_bottom_right.x < bottom_wall_top_left.x)) {
			return false;
		}
		else {
			// check if any corner is clipping into the walls
			if ((ap_top_left.x <= top_wall_bottom_right.x && ap_top_left.x >= top_wall_bottom_left.x && (ap_top_left.y <= top_wall_bottom_right.y || ap_top_left.y >= bottom_wall_top_right.y)) ||
				(ap_top_right.x <= top_wall_bottom_right.x && ap_top_right.x >= top_wall_bottom_left.x && (ap_top_right.y <= top_wall_bottom_right.y || ap_top_right.y >= bottom_wall_top_right.y)) ||
				(ap_bottom_left.x <= top_wall_bottom_right.x && ap_bottom_left.x >= top_wall_bottom_left.x && (ap_bottom_left.y >= bottom_wall_top_right.y || ap_bottom_left.y <= top_wall_bottom_right.y)) ||
				(ap_bottom_right.x <= top_wall_bottom_right.x && ap_bottom_right.x >= top_wall_bottom_left.x && (ap_bottom_right.y >= bottom_wall_top_right.y || ap_bottom_right.y <= top_wall_bottom_right.y))) {
				std:: << "Collision Detected!" << std::endl;


				return true;
			}
			else {
				glm::vec2 v1;
				glm::vec2 v2;
				glm::vec2 v3;
				// check if apple is going up or down
				if (ap_top_left.y > ap_top_right.y) {
					if (ap_top_right.y < top_wall_bottom_left.y && ap_top_right.x < top_wall_bottom_left.x) {
						v1 = glm::vec2(ap_bottom_right.x - ap_top_right.x, ap_bottom_right.y - ap_top_right.y);
						v2 = glm::vec2(top_wall_bottom_left.x - ap_top_right.x, top_wall_bottom_left.y - ap_top_right.y);
						v1 = v1 / glm::length(v1);
						v2 = v2 / glm::length(v2);
						v3 = v2 - v1;
						if (v3.y > 0) {
							return true;
						}
						else {
							return false;
						}
					}
					else if (ap_top_right.y < top_wall_bottom_left.y && ap_top_right.x > top_wall_bottom_right.x){
						v1 = glm::vec2(ap_top_left.x - ap_top_right.x, ap_top_left.y - ap_top_right.y);
						v2 = glm::vec2(top_wall_bottom_right.x - ap_top_right.x, top_wall_bottom_right.y - ap_top_right.y);
						v1 = v1 / glm::length(v1);
						v2 = v2 / glm::length(v2);
						v3 = v2 - v1;
						if (v3.y > 0) {
							return true;
						}
						else {
							return false;
						}
					}
					else if (ap_bottom_left.y > bottom_wall_top_left.y && ap_bottom_left.x < bottom_wall_top_left.x) {
						v1 = glm::vec2(ap_bottom_right.x - ap_bottom_left.x, ap_bottom_right.y - ap_bottom_left.y);
						v2 = glm::vec2(bottom_wall_top_left.x - ap_bottom_left.x, bottom_wall_top_left.y - ap_bottom_left.y);
						v1 = v1 / glm::length(v1);
						v2 = v2 / glm::length(v2);
						v3 = v2 - v1;
						if (v3.y < 0) {
							return true;
						}
						else {
							return false;
						}
					}
					else if (ap_bottom_left.y > bottom_wall_top_left.y && ap_bottom_left.x > bottom_wall_top_right.x) {
						v1 = glm::vec2(ap_top_left.x - ap_bottom_left.x, ap_top_left.y - ap_bottom_left.y);
						v2 = glm::vec2(bottom_wall_top_right.x - ap_bottom_left.x, bottom_wall_top_right.y - ap_bottom_left.y);
						v1 = v1 / glm::length(v1);
						v2 = v2 / glm::length(v2);
						v3 = v2 - v1;
						if (v3.y < 0) {
							return true;
						}
						else {
							return false;
						}
					}
					else {
						return false;
					}
				}
				else {
					if (ap_top_left.y < top_wall_bottom_left.y && ap_top_left.x < top_wall_bottom_left.x) {
						v1 = glm::vec2(ap_top_right.x - ap_top_left.x, ap_top_right.y - ap_top_left.y);
						v2 = glm::vec2(top_wall_bottom_left.x - ap_top_left.x, top_wall_bottom_left.y - ap_top_left.y);
						v1 = v1 / glm::length(v1);
						v2 = v2 / glm::length(v2);
						v3 = v2 - v1;
						if (v3.y > 0) {
							return true;
						}
						else {
							return false;
						}
					}
					else if (ap_bottom_right.y > bottom_wall_top_left.y && ap_bottom_right.x < bottom_wall_top_left.x) {
						v1 = glm::vec2(ap_top_right.x - ap_bottom_right.x, ap_top_right.y - ap_bottom_right.y);
						v2 = glm::vec2(bottom_wall_top_left.x - ap_bottom_right.x, bottom_wall_top_left.y - ap_bottom_right.y);
						v1 = v1 / glm::length(v1);
						v2 = v2 / glm::length(v2);
						v3 = v2 - v1;
						if (v3.y < 0) {
							return true;
						}
						else {
							return false;
						}
					}
					else if (ap_bottom_right.y > bottom_wall_top_left.y && ap_bottom_right.x > bottom_wall_top_right.x) {
						v1 = glm::vec2(ap_bottom_left.x - ap_bottom_right.x, ap_bottom_left.y - ap_bottom_right.y);
						v2 = glm::vec2(bottom_wall_top_right.x - ap_bottom_right.x, bottom_wall_top_right.y - ap_bottom_right.y);
						v1 = v1 / glm::length(v1);
						v2 = v2 / glm::length(v2);
						v3 = v2 - v1;
						if (v3.y < 0) {
							return true;
						}
						else {
							return false;
						}
					}
					else {
						return false;
					}
				}
			}
		}
	}
}
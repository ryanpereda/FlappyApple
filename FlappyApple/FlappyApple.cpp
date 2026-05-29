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
void applyHMovement(glm::mat4 hMovement, int hMovementLoc, Wall &wall);
void applyGravity();
bool checkCollision(Apple ap, glm::mat4 projection, glm::mat4 vMovement, glm::mat4 aRotate);

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
		applyVMovement(vMovement, vMovementLoc, aRotate, aRotateLoc);
		applyGravity();

		glUniform1i(glGetUniformLocation(ourShader.ID, "obj"), 0);
		glBindVertexArray(bg.getVAO());
		glDrawArrays(GL_TRIANGLES, 0, 6); // draw elements

		glUniform1i(glGetUniformLocation(ourShader.ID, "obj"), 1);
		for (int i = 0; i < walls.size(); i++) {
			applyHMovement(hMovement, hMovementLoc, walls[i]);
			
			glBindVertexArray(walls[i].getVAO());
			glDrawArrays(GL_TRIANGLES, 0, 12); // draw elements
		}
		if (walls[0].hPosition > 1400.0f) {
			walls[0].deleteObjects();
			walls.pop_front();
		}
		if (walls[0].hPosition > 700.0f && walls.size() == 1) {
			walls.push_back(Wall());
		}

		glUniform1i(glGetUniformLocation(ourShader.ID, "obj"), 2);
		glBindVertexArray(ap.getVAO());
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		for (int i = 0; i < walls.size(); i++) {
			if (1300.0f - walls[i].hPosition >= 585.0f - 85.0f && 1300.0f - walls[i].hPosition <= 695.0f + 10.0f) {
				checkCollision(ap, projection, vMovement, aRotate);
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

void applyHMovement(glm::mat4 hMovement, int hMovementLoc, Wall &wall) {
	wall.hPosition += H_VELOCITY * deltaTime;

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

bool checkCollision(Apple ap, glm::mat4 projection, glm::mat4 vMovement, glm::mat4 aRotate) {
	glm::vec4 bottom_left = glm::vec4(ap.vertices[0], ap.vertices[1], 0.0f, 1.0f);
	glm::vec4 top_left = glm::vec4(ap.vertices[6], ap.vertices[7], 0.0f, 1.0f);
	glm::vec4 bottom_right = glm::vec4(ap.vertices[12], ap.vertices[13], 0.0f, 1.0f);
	glm::vec4 top_right = glm::vec4(ap.vertices[18], ap.vertices[19], 0.0f, 1.0f);
	bottom_left = vMovement * aRotate * bottom_left;
	top_left = vMovement * aRotate * top_left;
	bottom_right = vMovement * aRotate * bottom_right;
	top_right = vMovement * aRotate * top_right;
	std::cout << "Bottom Left: " << bottom_left.x << ", " << bottom_left.y << std::endl;
	std::cout << "Top Left: " << top_left.x << ", " << top_left.y << std::endl;
	std::cout << "Bottom Right: " << bottom_right.x << ", " << bottom_right.y << std::endl;
	std::cout << "Top Right: " << top_right.x << ", " << top_right.y << std::endl;

	return true;
}
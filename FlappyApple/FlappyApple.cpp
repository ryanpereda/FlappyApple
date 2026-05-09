#include <iostream>
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
void applyMovement(glm::mat4 vMovement, int vMovementLoc, glm::mat4 hMovement, int hMovementLoc);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float vVelocity = 0.0f;
const float H_VELOCITY = 0.1f;
const float GRAVITY = -0.5f;
const float MAX_V_VELOCITY = 10.0f;
const float MIN_V_VELOCITY = -10.0f;

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
	Wall wall;

	ourShader.use();

	glm::mat4 vMovement = glm::mat4(1.0f);
	int vMovementLoc = glGetUniformLocation(ourShader.ID, "vMovement");
	

	glm::mat4 hMovement = glm::mat4(1.0f);
	int hMovementLoc = glGetUniformLocation(ourShader.ID, "hMovement");
	
	
	glm::mat4 projection;
	projection = glm::ortho(0.0f, float(SCR_WIDTH), float(SCR_HEIGHT), 0.0f, -1.0f, 1.0f);
	int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// activate shader
		ourShader.use();

		// texture units

		// matrices
		applyMovement(vMovement, vMovementLoc, hMovement, hMovementLoc);

		glUniform1i(glGetUniformLocation(ourShader.ID, "obj"), 0);
		glBindVertexArray(bg.getVAO());
		glDrawArrays(GL_TRIANGLES, 0, 6); // draw elements

		glUniform1i(glGetUniformLocation(ourShader.ID, "obj"), 1);
		glBindVertexArray(wall.getVAO());
		glDrawArrays(GL_TRIANGLES, 0, 12); // draw elements

		glUniform1i(glGetUniformLocation(ourShader.ID, "obj"), 2);
		glBindVertexArray(ap.getVAO());
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

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

void applyMovement(glm::mat4 vMovement, int vMovementLoc, glm::mat4 hMovement, int hMovementLoc) {
	vMovement = glm::translate(vMovement, glm::vec3(0.0f, 100.0f, 0.0f));
	glUniformMatrix4fv(vMovementLoc, 1, GL_FALSE, glm::value_ptr(vMovement));

	hMovement = glm::translate(hMovement, glm::vec3(100.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(hMovementLoc, 1, GL_FALSE, glm::value_ptr(hMovement));
}
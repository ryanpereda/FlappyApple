#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader_s.h"
#include "Background.h"
#include "Apple.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
	
	
	

	//float vertices[] {
	//	-1.0f, -1.0f, 0.0f, 0.53f, 0.81f, 0.92f,
	//	-1.0f,  1.0f, 0.0f, 0.53f, 0.81f, 0.92f,
	//	 1.0f,  1.0f, 0.0f, 0.53f, 0.81f, 0.92f,

	//	 1.0f,  1.0f, 0.0f, 0.53f, 0.81f, 0.92f,
	//	 1.0f, -1.0f, 0.0f, 0.53f, 0.81f, 0.92f,
	//	-1.0f, -1.0f, 0.0f, 0.53f, 0.81f, 0.92f
	//};

	//unsigned int VAO, VBO;
	//glGenVertexArrays(1, &VAO);
	//glBindVertexArray(VAO);
	//glGenBuffers(1, &VBO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	

	ourShader.use();

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

		glBindVertexArray(bg.getVAO());
		glDrawArrays(GL_TRIANGLES, 0, 6); // draw elements
		glBindVertexArray(0);

		glBindVertexArray(ap.getVAO());
		glDrawArrays(GL_TRIANGLES, 0, 6); // draw elements
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
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
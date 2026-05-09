#ifndef APPLE_H
#define APPLE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Apple {
public:
	unsigned int VAO, VBO, EBO;

	Apple() {
		float vertices[]{
			100.0f, 100.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
			100.0f, 200.0f, 0.0f, 1.0f, 0.0f, 0.0f, // top left
			200.0f, 100.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
			200.0f, 200.0f, 0.0f, 1.0f, 0.0f, 0.0f, // top right
		};

		unsigned int indices[] = {
			0, 1, 2, // first triangle
			1, 2, 3 // second triangle
		};

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);
	}
	int getVAO() {
		return VAO;
	}
	void deleteObjects() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
private:

};

#endif
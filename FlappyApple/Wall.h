#ifndef WALL_H
#define WALL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Wall {
public:
	unsigned int VAO, VBO;

	Wall() {
		float vertices[]{
			0.8f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.8f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.9f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

			0.8f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.9f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.9f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		};
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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
	}
private:

};

#endif
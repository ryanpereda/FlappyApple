#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Background {
public:
	unsigned int VAO, VBO;

	Background() {
		float vertices[]{
			-1.0f, -1.0f, 0.0f, 0.53f, 0.81f, 0.92f,
			-1.0f,  1.0f, 0.0f, 0.53f, 0.81f, 0.92f,
			 1.0f,  1.0f, 0.0f, 0.53f, 0.81f, 0.92f,

			 1.0f,  1.0f, 0.0f, 0.53f, 0.81f, 0.92f,
			 1.0f, -1.0f, 0.0f, 0.53f, 0.81f, 0.92f,
			-1.0f, -1.0f, 0.0f, 0.53f, 0.81f, 0.92f
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
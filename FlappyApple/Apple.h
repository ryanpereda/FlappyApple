#ifndef APPLE_H
#define APPLE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>


class Apple {
public:
	unsigned int VAO, VBO, EBO;
	float vertices[24];

	Apple() {
		float centerX = 640.0f;
		float startingY = 720.0f;
		float width = 72.0 / 2.0f; // also used for height

		float temp[] = {
			centerX - width, startingY + width, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
			centerX - width, startingY - width, 0.0f, 1.0f, 0.0f, 0.0f, // top left
			centerX + width, startingY + width, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
			centerX + width, startingY - width, 0.0f, 1.0f, 0.0f, 0.0f, // top right
		};
		memcpy(vertices, temp, sizeof(temp));

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

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
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
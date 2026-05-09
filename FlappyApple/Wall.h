#ifndef WALL_H
#define WALL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Wall {
public:
	unsigned int VAO, VBO;

	Wall() {
		float width = 75.0f;
		float coordX = 1000.0f;
		float minY = 0.0f;
		float maxY = 720.0f;
		float height = 360.0f - 125.0f;

		float vertices[]{
			coordX, minY, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
			coordX, minY + height, 0.0f, 0.0f, 1.0f, 0.0f, // top left
			coordX + width, minY, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right

			coordX, minY + height, 0.0f, 0.0f, 1.0f, 0.0f, // top left
			coordX + width, minY + height, 0.0f, 0.0f, 1.0f, 0.0f, // top right
			coordX + width, minY, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right


			coordX, maxY, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
			coordX, maxY - height, 0.0f, 0.0f, 1.0f, 0.0f, // top left
			coordX + width, maxY, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right

			coordX, maxY - height, 0.0f, 0.0f, 1.0f, 0.0f, // top left
			coordX + width, maxY - height, 0.0f, 0.0f, 1.0f, 0.0f, // top right
			coordX + width, maxY, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
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
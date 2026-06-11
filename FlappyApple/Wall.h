#ifndef WALL_H
#define WALL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <random>

class Wall {
public:
	unsigned int VAO, VBO;
	float hPosition;
	float rand;
	float height;
	float minY;
	float maxY;
	bool pointGiven;

	Wall() {
		float width = 75.0f;
		float coordX = 1300.0f;
		minY = 0.0f;
		maxY = 720.0f;
		height = 360.0f - 125.0f;
		hPosition = 0.0f;
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distr(0, 550);
		rand = float(distr(gen));
		rand -= 275.0f;
		pointGiven = false;

		float vertices[]{
			// top wall
			coordX, minY, 0.0f, 0.0f, 1.0f, 0.0f, // top left
			coordX, minY + height + rand, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
			coordX + width, minY, 0.0f, 0.0f, 1.0f, 0.0f, // top right

			coordX, minY + height + rand, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
			coordX + width, minY + height + rand, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
			coordX + width, minY, 0.0f, 0.0f, 1.0f, 0.0f, // top right

			// bottom wall
			coordX, maxY, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
			coordX, maxY - height + rand, 0.0f, 0.0f, 1.0f, 0.0f, // top left
			coordX + width, maxY, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right

			coordX, maxY - height + rand, 0.0f, 0.0f, 1.0f, 0.0f, // top left
			coordX + width, maxY - height + rand, 0.0f, 0.0f, 1.0f, 0.0f, // top right
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
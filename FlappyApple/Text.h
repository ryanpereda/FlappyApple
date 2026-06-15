#ifndef TEXT_H
#define TEXT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

class Text {
public:
	struct Character {
		unsigned int TextureID;
		glm::ivec2 Size;
		glm::ivec2 Bearing;
		unsigned int Advance;
	};
	std::map<char, Character> Characters;

	unsigned int VAO, VBO;

	Text() {
		FT_Library ft;
		if (FT_Init_FreeType(&ft)) {
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		}


		FT_Face face;
		if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
		{
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		}

		FT_Set_Pixel_Sizes(face, 0, 48);

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
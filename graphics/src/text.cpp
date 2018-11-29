#include "text.hpp"
#include <iostream>
#include <gl/glew.h>

namespace graphics
{

Text::Text()
{
	FT_Error error = FT_Init_FreeType(&library);
	if (error)
	{
		std::cout << "error occurred during library initialization ...";
	}

	FT_Face face;
	if (FT_New_Face(library, "../resources/fonts/theboldfont.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	FT_Set_Pixel_Sizes(face, 0, 48);

	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
		std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);// Disable byte-alignment restriction

	for (auto c = 0ul; c < 128; ++c)
	{
		// Load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
		}
		else
		{
			// Generate texture
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Now store character for later use
			character new_character =
			{
				texture,
				face->glyph->advance.x,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top)
			};

			characters[c] = new_character;
		}
	}

	FT_Done_Face(face);
	FT_Done_FreeType(library);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4,
		NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Text::~Text()
{

}

void Text::render_text(const std::string& text,
	float x, float y, float scale)const
{
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	for (auto& c : text)
	{
		auto& ch = characters[c];

		auto xpos = x + ch.bearing.x * scale;
		auto ypos = y - (ch.size.y - ch.bearing.y) * scale;
		auto w = ch.size.x * scale;
		auto h = ch.size.y * scale;

		float vertices[6][4] =
		{
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};

		glBindTexture(GL_TEXTURE_2D, ch.texture_index);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Bitshift by 6 to get value in pixels (2^6 = 64)
		x += (ch.advance >> 6) * scale;
	}
}

float Text::get_text_width(const std::string& text, float scale)const
{
	float w = 0;
	for (auto& c : text)
	{
		auto& ch = characters[c];

		if (c != '.')
		{
			w += 25 * scale;
		}
		else
			w += ch.size.x * scale;
	}

	return w;
}

}

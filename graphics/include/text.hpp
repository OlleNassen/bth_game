#ifndef TEXT_HPP
#define TEXT_HPP
#include <string>
#include <array>
#include <glm/glm.hpp>
#include <ft2build.h>
#include "camera.hpp"
#include "shader.hpp"
#include FT_FREETYPE_H

namespace graphics
{

struct character
{
	unsigned int texture_index;  // ID handle of the glyph texture
	long int advance;    // Offset to advance to next glyph
	glm::ivec2 size;       // Size of glyph
	glm::ivec2 bearing;    // Offset from baseline to left/top of glyph
};

class Text
{
public:
	Text();
	~Text();
	void render_text(const std::string& text,
		float x, float y, float scale)const;

	float get_text_width(const std::string& text, float scale)const;
private:
	FT_Library library;
	unsigned int VAO, VBO;
	std::array<character, 128> characters;
};

}

#endif
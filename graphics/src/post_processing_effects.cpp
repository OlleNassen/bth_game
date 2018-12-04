#include "post_processing_effects.hpp"
#include <cmath>

namespace graphics
{

PostProcessingEffects::PostProcessingEffects()
:texture("../resources/textures/screen_warning.png", true)
{
	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);
	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(primitives::quad_uv), &primitives::quad_uv, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
	glEnableVertexAttribArray(0);


}

PostProcessingEffects::~PostProcessingEffects()
{
}

void PostProcessingEffects::render() const
{
	glBindVertexArray(vao_id);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void PostProcessingEffects::update(std::chrono::milliseconds delta)
{
	duration += delta;
	glow_value = std::cos(4 * duration.count());
}

void PostProcessingEffects::initialize_textures()
{

}

}

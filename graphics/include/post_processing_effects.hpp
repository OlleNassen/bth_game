#ifndef POST_PROCESSING_EFFECTS_HPP
#define POST_PROCESSING_EFFECTS_HPP
#include <GL/glew.h>
#include <iostream>
#define BUFFER_OFFSET(i) ((char *)NULL + (i)) 

class PostProcessingEffects
{
public:
	PostProcessingEffects();
	~PostProcessingEffects();

	void render()const;

private:

	unsigned int vao_id;
	unsigned int vbo_id;
};

#endif
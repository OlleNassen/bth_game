#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <array>
#include <vector>
#include <gl/glew.h>
namespace graphics
{

class Buffer
{
public:
	Buffer()
	{
		glGenBuffers(1, &id);
	}
	~Buffer()
	{
		glDeleteBuffers(1, &id);
	}
	operator GLuint() const
	{
		return id;
	}

private:
	GLuint id;
};

class VertexArray
{
public:
	VertexArray()
	{
		glGenVertexArrays(1, &id);
	}
	~VertexArray()
	{
		glDeleteVertexArrays(1, &id);
	}
	operator GLuint() const
	{
		return id;
	}

private:
	GLuint id;
};


template <typename T, int N>
void gl_buffer_data(GLenum target, std::array<T, N> data, GLenum usage)
{
	glBufferData(target, sizeof(T) * data.size(), data.data(), usage);
}

template <typename T>
void gl_buffer_data(GLenum target, std::vector<T> data, GLenum usage)
{
	glBufferData(target, sizeof(T) * data.size(), data.data(), usage);
}

}

#endif

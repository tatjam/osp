#pragma once
#include "shader.h"
class Framebuffer
{
private:
	size_t width, height;

public:

	// GL framebuffer object
	GLuint fbuffer;
	// The texture color buffer you can actually read
	// -> Use this with glBindTexture to use it as a texture <-
	GLuint tex_color_buffer;
	// The renderbuffer used internally to store depth and stencil
	GLuint rbo;

	// Binds the framebuffer for rendering to it
	void bind();

	// Unbinds the framebuffer, returning it to
	// 0 (not to the previous framebuffer)
	void unbind();

	void set_viewport();

	glm::ivec2 get_size();

	Framebuffer(size_t width, size_t height);
	~Framebuffer();
};


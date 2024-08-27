#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <glad/glad.h>

#include "shader.h"

#include <iostream>

class FrameBuffer
{
public:
	unsigned int id;
	unsigned int textureId; //uint for color attachment texture
	unsigned int RBO;
	unsigned int ColorBuffers;

	FrameBuffer(unsigned int screenWidth, unsigned int screenHeight, unsigned int rbo, unsigned int colorBuffers, unsigned int colorAttach);

	void Bind() const;


private:
	void Generate(unsigned int screenWidth, unsigned int screenHeight, unsigned int colorAttach);

};

#endif
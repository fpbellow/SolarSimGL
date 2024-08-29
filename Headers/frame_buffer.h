#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <glad/glad.h>

#include "shader.h"

#include <iostream>
#include <vector>

class FrameBuffer
{
public:
	unsigned int id[2];
	unsigned int textureId[2]; //uint for color attachment texture
	unsigned int RBO;
	unsigned int ColorBuffers;
	unsigned int NumFBOs;

	FrameBuffer(unsigned int screenWidth, unsigned int screenHeight, unsigned int rbo, unsigned int numFBOs, unsigned int colorBuffers, unsigned int colorAttach);

	void Bind(int index) const;


private:
	void Generate(unsigned int screenWidth, unsigned int screenHeight, unsigned int colorAttach);

};

#endif
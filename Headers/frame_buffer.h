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
	unsigned int screenVAO;

	FrameBuffer(unsigned int screenWidth, unsigned int screenHeight);

	void Bind() const;

	void Draw(Shader shader);

private:
	void Generate(unsigned int screenWidth, unsigned int screenHeight);

};

#endif
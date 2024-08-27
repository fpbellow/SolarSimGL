#include "../Headers/frame_buffer.h"

FrameBuffer::FrameBuffer(unsigned int screenWidth, unsigned int screenHeight, unsigned int rbo, unsigned int colorBuffers, unsigned int colorAttach) : ColorBuffers(colorBuffers)
{
	this->RBO = rbo;
	Generate(screenWidth, screenHeight, colorAttach);
}

void FrameBuffer::Generate(unsigned int screenWidth, unsigned int screenHeight, unsigned int colorAttach)
{
	//generate and bind frame buffer
	glGenFramebuffers(1, &this->id);
	glBindFramebuffer(GL_FRAMEBUFFER, this->id);

	//generate and bind color attachment texture
	glGenTextures(1, &this->textureId);
	glBindTexture(GL_TEXTURE_2D, this->textureId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->textureId, 0);

	//render buffer object
	
	glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->RBO);

	//check if framebuffer complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete." << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void FrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->id);
	glEnable(GL_DEPTH_TEST);
}


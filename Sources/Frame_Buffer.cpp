#include "../Headers/frame_buffer.h"

FrameBuffer::FrameBuffer(unsigned int screenWidth, unsigned int screenHeight, unsigned int rbo,  unsigned int numFBOs, unsigned int colorBuffers, unsigned int colorAttach) : ColorBuffers(colorBuffers), NumFBOs(numFBOs)
{
	this->RBO = rbo;
	Generate(screenWidth, screenHeight, colorAttach);
}

void FrameBuffer::Generate(unsigned int screenWidth, unsigned int screenHeight, unsigned int colorAttach)
{
	//generate and bind frame buffer
	if (this->NumFBOs > 1)
	{
		glGenFramebuffers(2, this->id);
		for (unsigned int i = 0; i < this->NumFBOs; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, this->id[0]);
		}
	}
	else
	{
		glGenFramebuffers(1, &this->id[0]);
		glBindFramebuffer(GL_FRAMEBUFFER, this->id[0]);
	}
	

	//generate and bind color attachment textures
	if (this->ColorBuffers > 1)
	{
		glGenTextures(2, this->textureId);
		for (unsigned int i = 0; i < this->ColorBuffers; i++)
		{
			glBindTexture(GL_TEXTURE_2D, this->textureId[i]);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			if (colorAttach > 0)
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, this->textureId[i], 0);
			else
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttach, GL_TEXTURE_2D, this->textureId[i], 0);
		}
	}
	else
	{
		glGenTextures(1, &this->textureId[0]);
		glBindTexture(GL_TEXTURE_2D, this->textureId[0]);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+colorAttach, GL_TEXTURE_2D, this->textureId[0], 0);
	}


	//render buffer object binding
	if (this->RBO != 0)
	{
		glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->RBO);
	}
	

	if (colorAttach > 0)
	{
		unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, attachments);
	}

	//check if framebuffer complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete." << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void FrameBuffer::Bind(int index) const
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->id[index]);
	glEnable(GL_DEPTH_TEST);
}


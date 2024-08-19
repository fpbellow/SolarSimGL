#include "../Headers/cubemap.h"

Cubemap::Cubemap() 
	: Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_CLAMP_TO_EDGE), Wrap_T(GL_CLAMP_TO_EDGE), Wrap_R(GL_CLAMP_TO_EDGE), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
{
	glGenTextures(1, &this->id);
}


void Cubemap::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->id);
}
#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

class Texture2D
{
public:
	//texture object id used for operations
	unsigned int id;

	//width and height of loaded image in pixels
	unsigned int Width, Height;

	//texture format
	unsigned int Internal_Format;
	unsigned int Image_Format;

	//texture configuration
	unsigned int Wrap_S;
	unsigned int Wrap_T;
	unsigned int Filter_Min;
	unsigned int Filter_Max;

	//constructor
	Texture2D();

	//generate texture from image data
	void Generate(unsigned int width, unsigned int height, unsigned char* data);

	//bind texture as current active GL_TEXTURE_2D
	void Bind() const;

};

#endif
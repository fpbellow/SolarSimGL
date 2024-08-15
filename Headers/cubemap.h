#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <glad/glad.h>

#include "shader.h"

#include <string>
#include <vector>

class Cubemap {
public:
	unsigned int id;

	//width and height of loaded image in pixels
	unsigned int Width, Height;

	//texture format
	unsigned int Internal_Format;
	unsigned int Image_Format;

	//texture configuration
	unsigned int Wrap_S;
	unsigned int Wrap_T;
	unsigned int Wrap_R;
	unsigned int Filter_Min;
	unsigned int Filter_Max;

	Cubemap();

	void Generate(unsigned int width, unsigned int height, std::vector<unsigned char*> data);

	void Bind() const;
};

#endif
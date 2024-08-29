#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float exposure;

uniform bool bloom;
uniform sampler2D bloomBlur;

void main()
{
	const float gamma = 1.6;
	vec3 hdrColor = vec3(texture(screenTexture, TexCoords));
	vec3 bloomColor = vec3(texture(bloomBlur, TexCoords));


	hdrColor += bloomColor;
	
	//tonemapping
	vec3 result = vec3(1.0) - exp(-hdrColor * exposure);

	//gamma correction
	//result = pow(result, vec3(1.0/gamma)); //currently conflicts with earth atmo gamma correction

	FragColor = vec4(result, 1.0);
}
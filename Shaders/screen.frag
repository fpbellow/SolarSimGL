#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float exposure;

void main()
{
	const float gamma = 1.6;
	vec3 hdrColor = vec3(texture(screenTexture, TexCoords));
	
	vec3 result = vec3(1.0) - exp(-hdrColor * exposure);

	//gamma correction
	//result = pow(result, vec3(1.0/gamma));

	FragColor = vec4(result, 1.0);
}
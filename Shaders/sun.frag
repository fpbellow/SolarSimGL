#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse;
uniform vec3 sunColor;

void main() 
{
	vec3 sunTexture = texture(texture_diffuse, TexCoords).rgb;
	sunTexture *= sunColor;
	FragColor = vec4(sunTexture, 1.0);
}

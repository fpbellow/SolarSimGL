#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;

vec3 color = vec3(0.4, 0.2, 0.9);

void main() 
{
	FragColor = vec4(color, 0.2);
}

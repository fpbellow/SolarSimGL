#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 viewPos;

vec3 color = vec3(0.4, 0.2, 0.9);

void main() 
{
	float distanceToCenter = length(gl_FragCoord.xyz - vec3(0.0));
	if(distanceToCenter < 1.0)
	{
		FragColor = vec4(color, 0.2);
	}
	else 
	{
		FragColor = vec4(0.0);
	}

	
}

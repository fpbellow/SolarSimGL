#version 420 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse;
uniform vec3 sunColor;

void main() 
{
	vec3 sunTexture = texture(texture_diffuse, TexCoords).rgb;
	sunTexture *= sunColor;
	FragColor = vec4(sunTexture, 1.0);
	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	if(brightness > 1.0)
		BrightColor = vec4(FragColor.rgb, 1.0);
	else	
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}

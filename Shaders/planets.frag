#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

struct Material 
{
	sampler2D diffuse;
	sampler2D specular;
	float shineFactor;

	bool earth;
	sampler2D textureLayer1;
	sampler2D textureLayer2;
};

struct Light 
{
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;


uniform vec3 viewPos;
uniform float u_time;

uniform Material PlanetMtl;
uniform Light SunLight;

void main() 
{

	//ambient lighting
	vec3 ambient = SunLight.ambient * vec3(texture(PlanetMtl.diffuse, TexCoords));

	//diffuse lighting
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(SunLight.position - FragPos);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	

	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = SunLight.diffuse * diff * vec3(texture(PlanetMtl.diffuse, TexCoords));


	if(PlanetMtl.earth)
	{
		//nightmap
		float aCoef = max(dot(-lightDir, norm-0.5), 0.0);
		vec3 nightMap = SunLight.diffuse* aCoef * vec3(texture(PlanetMtl.textureLayer1, TexCoords));
		diffuse += (1.0 - diff) < 0.7 ? vec3(0.0) : nightMap;

		//cloud layer
		float diffTwo = max(diff, ambient.x * 2.0);
		vec3 cloudMap = diffTwo * vec3(texture(PlanetMtl.textureLayer2, TexCoords + vec2(-u_time * 0.001,0.0)));
		//vec3 cloudMap = diffTwo * vec3(texture(PlanetMtl.textureLayer2, TexCoords));
		diffuse += cloudMap;
	}

	//specular lighting
	//vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(norm, halfwayDir), 0.0), PlanetMtl.shineFactor);
	vec3 specular =SunLight.specular * spec * vec3(texture(PlanetMtl.specular, TexCoords));


	//attenuation
	float distance = length(SunLight.position - FragPos);
	float attenuation = 1.0/ (SunLight.constant + SunLight.linear * distance +
		SunLight.quadratic * (distance * distance));


	//blinn-phong lighting combination
	vec3 result = (specular + diffuse) * attenuation;

	//bloom threshold check
	float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
	if(brightness > 1.0)
		BrightColor = vec4(result, 1.0);
	else 
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
	FragColor = vec4(result, 1.0);
}

#version 330 core
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

out vec4 FragColor;

uniform vec3 viewPos;


uniform Material PlanetMtl;
uniform Light SunLight;

void main() 
{

	//ambient lighting
	vec3 ambient = SunLight.ambient * vec3(texture(PlanetMtl.diffuse, TexCoords));

	//diffuse lighting
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(SunLight.position - FragPos);
	
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = SunLight.diffuse * diff * vec3(texture(PlanetMtl.diffuse, TexCoords));


	if(PlanetMtl.earth)
	{
		//nightmap
		float aCoef = max(dot(-lightDir, norm), 0.0);
		diffuse += SunLight.diffuse * aCoef * vec3(texture(PlanetMtl.textureLayer1, TexCoords));

		//cloud layer
		float diffTwo = max(diff, 0.33);
		vec3 atmosphere = SunLight.diffuse * diffTwo * vec3(texture(PlanetMtl.textureLayer2, TexCoords));
		diffuse += atmosphere;
	}

	//specular lighting
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), PlanetMtl.shineFactor);
	vec3 specular =SunLight.specular * spec * vec3(texture(PlanetMtl.specular, TexCoords));

	
	

	//attenuation
	float distance = length(SunLight.position - FragPos);
	float attenuation = 1.0/ (SunLight.constant + SunLight.linear * distance +
		SunLight.quadratic * (distance * distance));


	//phong lighting combination
	vec3 result = ambient + diffuse + (specular * attenuation);

	FragColor = vec4(result, 1.0);
}

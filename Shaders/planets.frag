#version 330 core
struct Material 
{
	sampler2D diffuse;
	sampler2D specular;
	float shineFactor;
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

uniform Material FragMaterial;
uniform Light FragLight;

void main() 
{

	//ambient lighting
	vec3 ambient = FragLight.ambient * vec3(texture(FragMaterial.diffuse, TexCoords));

	//diffuse lighting
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(FragLight.position - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = FragLight.diffuse * diff * vec3(texture(FragMaterial.diffuse, TexCoords));
	
	//specular lighting
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), FragMaterial.shineFactor);
	vec3 specular =FragLight.specular * spec * vec3(texture(FragMaterial.specular, TexCoords));
	

	//attenuation
	float distance = length(FragLight.position - FragPos);
	float attenuation = 1.0/ (FragLight.constant + FragLight.linear * distance +
		FragLight.quadratic * (distance * distance));


	//phong lighting combination
	vec3 result = ambient + diffuse + (specular * attenuation);

	FragColor = vec4(result, 1.0);
}

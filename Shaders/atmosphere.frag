#version 420 core
//reference: https://glitch.com/~polite-playful-wool by Brian Jackson
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

struct Material 
{
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

//out vec4 FragColor;

uniform vec3 viewPos;

uniform Material PlanetMtl;
uniform Light SunLight;


float surfaceRadius = 0.4505;
float atmoRadius = 0.4785;

// math const
const float PI = 3.14159265359;
const float MAX = 10000.0;

const int NUM_OUT_SCATTER = 8;
const int NUM_IN_SCATTER = 80;


///////////////////

//e = -b +/- sqrt(b^2 - c)
vec2 ray_sphere_intersect(vec3 rayOrigin, vec3 rayDirection, float sphereRadius)
{
	float b = dot(rayOrigin, rayDirection);
	float c = dot(rayOrigin, rayOrigin) - sphereRadius * sphereRadius;

	float d = b * b - c;
	if(d < 0.0)
	{
		return vec2(MAX, -MAX);
	}
	d = sqrt(d);

	return vec2(-b-d, -b+d);
}

// Rayleigh
// g : 0
// F = 3/16PI * ( 1 + c^2 )
float rayleigh_phase(float cc)
{
	return (3.0 / 16.0 / PI) * (1.0 + cc);
}


// Mie
// g : ( -0.75, -0.999 )
//      3 * ( 1 - g^2 )               1 + c^2
// F = ----------------- * -------------------------------
//      8pi * ( 2 + g^2 )     ( 1 + g^2 - 2 * g * c )^(3/2)
float mie_phase(float g, float c, float cc)
{
	float gg = g * g;

	float a = (1.0 - gg) * (1.0 + cc);

	float b = 1.0 + gg - 2.0 * g * c;
	b *= sqrt(b);
	b *= 2.0+ gg;

	return (3.0/ 8.0 / PI) * a/b;
}

float density(vec3 p, float ph)
{
	float altitude = length(p) - surfaceRadius;
	float rho_0 = 1.225;

	float rho = rho_0 * exp(-max(altitude, 0.0));
	return rho * ph * 1.66;
}

float optic(vec3 p, vec3 q, float ph)
{
	vec3 s = (q - p) / float(NUM_OUT_SCATTER);
	vec3 v = p + s * 0.5;

	float sum = 0.0;
	for (int i = 0; i < NUM_OUT_SCATTER; i++) 
	{
		sum += density(v, ph);
		v += s;
	}
	sum *= length(s);

	return sum;
}

vec4 in_scatter(vec3 o, vec3 dir, vec2 e, vec3 l, float l_intensity)
{
	const float ph_ray = 0.35;
    const float ph_mie = 0.25;
	const float ph_alpha = 0.35;

    const vec3 k_ray = vec3(3.8, 13.5, 33.1);
    const vec3 k_mie = vec3(21.0);
    const float k_mie_ex = 1.1;
          
    const float k_alpha = 2.0;
	
	vec3 sum_ray = vec3(0.0);
    vec3 sum_mie = vec3(0.0);
    float sum_alpha = 0.0;

    float n_ray0 = 0.0;
    float n_mie0 = 0.0;

	float len = (e.y - e.x) / float(NUM_IN_SCATTER);
	vec3 s = dir * len;
	vec3 v = o + dir * (e.x + len * 0.5);

	for(int i = 0; i < NUM_IN_SCATTER; i++, v +=s)
	{
		float d_ray = density(v, ph_ray) * len;
		float d_mie = density(v, ph_mie) * len;
		float d_alpha = density(v, ph_alpha) * len;

		n_ray0 += d_ray;
		n_mie0 += d_mie;

		vec2 f = ray_sphere_intersect(v, l, atmoRadius);
		vec3 u = v + l * f.y;

		float n_ray1 = optic(v, u, ph_ray);
		float n_mie1 = optic(v, u, ph_mie);

		vec3 att = exp(-(n_ray0 + n_ray1) * k_ray - (n_mie0 + n_mie1) * k_mie * k_mie_ex);

		sum_ray += d_ray * att;
		sum_mie += d_mie * att;

		sum_alpha += d_alpha;
	}

	float c = dot(dir, -l);
	float cc = c * c;
	vec3 scatter = sum_ray * k_ray * rayleigh_phase(cc) 
		+ sum_mie * k_mie * mie_phase(-0.8, c, cc);

	float alpha = sum_alpha * k_alpha;
	return vec4(scatter * l_intensity, alpha);
}

void main() 
{
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(FragPos - SunLight.position);
		vec3 viewDir = normalize(viewPos - FragPos);


		float distance = length(SunLight.position - FragPos);
		float attenuation = 1.0/ (SunLight.constant + SunLight.linear * distance +
		SunLight.quadratic * (distance * distance));

		vec2 e = ray_sphere_intersect(FragPos.xyz, -viewDir, atmoRadius);

		vec2 f = ray_sphere_intersect(FragPos.xyz, -viewDir, surfaceRadius);
		e.y = min(e.y, f.x);
		
		vec4 I = in_scatter(FragPos.xyz, -viewDir, e, -lightDir, attenuation );

		vec4 result = vec4(1.0) - exp(-I * 1.95);
		vec4 I_gamma = pow(result, vec4(1.0/ 1.65));

		float brightness = dot(I_gamma.rgb, vec3(0.2126, 0.7152, 0.0722));
		if(brightness > 1.0)
			BrightColor = vec4(I_gamma.rgb, 1.0);
		else 
			BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
		FragColor = I_gamma;
}

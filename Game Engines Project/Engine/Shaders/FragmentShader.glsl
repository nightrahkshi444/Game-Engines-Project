#version 450 core

in vec3 Normal;
in vec2 TexCoords;
in vec3 Colour;
in vec3 FragPosition;

uniform sampler2D inputTexture;

out vec4 fColour;

uniform vec3 cameraPos;
uniform Light light;

struct Light()
{
	vec3 lightPos;
	float ambient;
	float diffuse;
	float specular;
	vec3 lightColor;
}

void main()
{
	vec3 ambient = light.ambient * texture(inputTexture, TexCoords).rgb * light.lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.lightPos - FragPosition);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = (diff * light.diffuse) * texture(inputTexture, TexCoords).rgb * light.lightColor;

	vec3 viewDir = normalize(viewPosition - FragPosition);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = (spec * light.specular) * light.lightColor;

	vec3 result = ambient + diffuse + specular;
	fColour = vec4(result, 1.0f);
}
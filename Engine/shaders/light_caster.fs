#version 430 core
out vec4 FragColor;

struct Camera {
	vec3 position;

	mat4 view;
	mat4 projection;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess = 32;
};

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

uniform Camera camera;
uniform Light light;
uniform Material material;

void main() {
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, vTexCoord));

	vec3 nNormal = normalize(vNormal);
	vec3 fragLightDir = normalize(light.position - vPosition.xyz);
	vec3 diffuse = max(dot(nNormal, fragLightDir), 0.0) * light.diffuse * vec3(texture(material.diffuse, vTexCoord));

	vec3 fragCameraDir = normalize(camera.position - vPosition.xyz);
	vec3 reflectDir = reflect(-fragLightDir, nNormal);
	vec3 specular = pow(max(dot(fragCameraDir, reflectDir), 0.0), material.shininess) * light.specular * vec3(texture(material.specular, vTexCoord));

	FragColor = vec4(ambient + diffuse + specular, 1.0);
}

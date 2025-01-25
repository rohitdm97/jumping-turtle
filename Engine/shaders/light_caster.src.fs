#version 430 core
out vec4 FragColor;

struct Camera {
	vec3 position;

	mat4 view;
	mat4 projection;
};

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

uniform sampler2D diffuse;
uniform Camera camera;
uniform Light light;

void main() {
	vec3 lc = (light.ambient + light.diffuse + light.specular)/3;
	float factor = 0.3;
	float r = 1 - factor + factor * lc.x;
	float g = 1 - factor + factor * lc.y;
	float b = 1 - factor + factor * lc.z;
	FragColor = vec4(r, g, b, 1.0);
}

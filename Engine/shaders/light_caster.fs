#version 430 core
out vec4 FragColor;

struct Camera {
	vec3 position;

	mat4 view;
	mat4 projection;
};

#define NO_OF_LIGHTS 8

#define DIRECTION_LIGHT 1
#define POINT_LIGHT 2
#define SPOT_LIGHT 4

struct Light {
	int type;
	bool enabled;

	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// direction
	vec3 direction;

	// point light
	float constant;
	float linear;
	float quadratic;

	// spot light
	float cutoff;
	float outerCutoff;
};

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
	sampler2D normals;
};

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

uniform Camera camera;
uniform int lights_length = 0;
uniform Light lights[NO_OF_LIGHTS];
uniform Material material;

vec3 Calc_direction_light(Light l1);
vec3 Calc_point_light(Light l1);
vec3 Calc_spot_light(Light l1);

void main() {
	vec3 result = vec3(0.0);
	for (int i = 0; i < lights_length; i++) {
		if (!lights[i].enabled) {
			continue;
		}
		switch (lights[i].type) {
			case DIRECTION_LIGHT:
				result += Calc_direction_light(lights[i]);
				break;
			case POINT_LIGHT:
				result += Calc_point_light(lights[i]);
				break;
			case SPOT_LIGHT:
				result += Calc_spot_light(lights[i]);
				break;
		}
	}

	FragColor = vec4(result, 1.0);
}

vec3 Calc_direction_light(Light l1) {
	vec3 nNormal = normalize(vNormal);
	// vec3 nNormal = normalize(texture(material.normals, vTexCoord)).xyz;
	vec3 fragLightDir = normalize(-l1.direction);
	vec3 reflectDir = reflect(-fragLightDir, nNormal);

	vec3 ambient = l1.ambient * vec3(texture(material.diffuse, vTexCoord));
	vec3 diffuse = max(dot(nNormal, fragLightDir), 0.0) * l1.diffuse * vec3(texture(material.diffuse, vTexCoord));

	vec3 fragCameraDir = normalize(camera.position - vPosition.xyz);
	vec3 specular = pow(max(dot(fragCameraDir, reflectDir), 0.0), material.shininess) * l1.specular * vec3(texture(material.specular, vTexCoord));

	return ambient + diffuse + specular;
}

vec3 Calc_point_light(Light l1) {
	vec3 nNormal = normalize(vNormal);
	// vec3 nNormal = normalize(texture(material.normals, vTexCoord)).xyz;
	vec3 fragLightDir = normalize(l1.position - vPosition.xyz);
	vec3 reflectDir = reflect(-fragLightDir, nNormal);

	float distance = length(l1.position - vPosition.xyz);
	float attenuation = 1.0 / (l1.constant + l1.linear * distance + l1.quadratic * distance * distance);

	vec3 ambient = l1.ambient * vec3(texture(material.diffuse, vTexCoord));
	vec3 diffuse = max(dot(nNormal, fragLightDir), 0.0) * l1.diffuse * vec3(texture(material.diffuse, vTexCoord));

	vec3 fragCameraDir = normalize(camera.position - vPosition.xyz);
	vec3 specular = pow(max(dot(fragCameraDir, reflectDir), 0.0), material.shininess) * l1.specular * vec3(texture(material.specular, vTexCoord));

	return attenuation * (ambient + diffuse + specular);
}

vec3 Calc_spot_light(Light l1) {
	vec3 nNormal = normalize(vNormal);
	// vec3 nNormal = normalize(texture(material.normals, vTexCoord)).xyz;
	vec3 fragLightDir = normalize(l1.position - vPosition.xyz);
	vec3 reflectDir = reflect(-fragLightDir, nNormal);

	float theta = dot(fragLightDir, normalize(-l1.direction));
	float epsilon = l1.cutoff - l1.outerCutoff;
	float intensity = clamp((theta - l1.outerCutoff) / epsilon, 0.0, 1.0);

	float distance = length(l1.position - vPosition.xyz);
	float attenuation = 1.0 / (l1.constant + l1.linear * distance + l1.quadratic * distance * distance);

	vec3 ambient = attenuation * l1.ambient * vec3(texture(material.diffuse, vTexCoord));
	vec3 diffuse = intensity * attenuation * max(dot(nNormal, fragLightDir), 0.0) * l1.diffuse * vec3(texture(material.diffuse, vTexCoord));

	vec3 fragCameraDir = normalize(camera.position - vPosition.xyz);
	vec3 specular = intensity * attenuation * pow(max(dot(fragCameraDir, reflectDir), 0.0), material.shininess) * l1.specular * vec3(texture(material.specular, vTexCoord));

	return ambient + diffuse + specular;
}

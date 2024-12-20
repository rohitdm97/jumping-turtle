#version 430 core

struct Camera {
	vec3 position;

	mat4 view;
	mat4 projection;
};

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform mat4 Mesh;
uniform mat4 Model;
uniform Camera camera;

out vec4 vPosition;
out vec3 vNormal;
out vec2 vTexCoord;

void main() {
	gl_Position = camera.projection * camera.view * Model * Mesh * vec4(Position, 1.0);
	vTexCoord = TexCoord;
	vNormal = Normal;
	vPosition = Model * Mesh * vec4(Position, 1.0);
}


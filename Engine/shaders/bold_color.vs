#version 430 core

struct Camera {
	vec3 position;

	mat4 view;
	mat4 projection;
};

in vec3 Position;

uniform mat4 Mesh;
uniform mat4 Model;
uniform Camera camera;

void main() {
	gl_Position = camera.projection * camera.view * Model * Mesh * vec4(Position, 1.0);
}


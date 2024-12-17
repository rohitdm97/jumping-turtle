#version 430 core

in vec3 Position;
in vec2 TexCoord;

uniform mat4 Mesh;
uniform mat4 Model;
uniform mat4 CameraView;
uniform mat4 CameraProjection;

out vec4 vPosition;
out vec2 vTexCoord;

void main() {
	gl_Position = CameraProjection * CameraView * Model * Mesh * vec4(Position, 1.0);
	vTexCoord = TexCoord;
	vPosition = CameraView * Model * vec4(Position, 1.0);
}


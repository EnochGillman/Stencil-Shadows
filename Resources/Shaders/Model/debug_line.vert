#version 460 core
layout (location = 0) in vec3 inPos;

uniform vec3 unColor = vec3(1.0f);
uniform mat4 unView;

out vec3 ouColor;

void main() {
	gl_Position = unView * vec4(inPos.x, inPos.y, inPos.z, 1.0f);
	ouColor = unColor;
}

#version 460 core
out vec4 FragColor;

in float ouFog;

void main() {
	FragColor = vec4(vec3(ouFog), 1.0f);
}

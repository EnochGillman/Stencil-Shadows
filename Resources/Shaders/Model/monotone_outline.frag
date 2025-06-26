#version 460 core
out vec4 FragColor;

in float ouFog;

uniform vec3 unColor;
uniform vec3 unLightAc;

void main() {
	FragColor = mix(
		vec4(unColor, 1.0f),
		vec4(unLightAc, 1.0f),
		ouFog
	);
}

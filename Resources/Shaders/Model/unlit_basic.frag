#version 460 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 FogColor;

in DATA {
	vec3 Pos;
	vec3 Normal;
	vec2 TexCoord;
	float Fog;
} ou;

struct Mat {
	sampler2D diff1;
	sampler2D spec1;
};

uniform Mat unMat;

void main() {
	FragColor = texture(unMat.diff1, ou.TexCoord);
	FogColor = vec4(vec3(ou.Fog), 1.0f);
}

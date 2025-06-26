#version 460 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

out DATA {
	vec3 Pos;
	vec3 Normal;
	vec2 TexCoord;
} ou;

uniform mat4 unMesh;
uniform mat4 unModel;
uniform mat4 unView;

void main() {
	ou.Pos = vec3(unModel * unMesh * vec4(inPos, 1.0f));
	gl_Position = unView * vec4(ou.Pos, 1.0f);
	ou.Normal = normalize(vec3(mat4(mat3(unModel * unMesh)) * vec4(inNormal, 1.0f)));
	ou.TexCoord = inTexCoord;
}

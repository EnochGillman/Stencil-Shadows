#version 460 core
layout (location = 0) in vec3 inPos;

out float ouFog;

uniform mat4 unMesh;
uniform mat4 unModel;
uniform mat4 unView;
uniform vec3 unViewPos;
uniform float unClipNear;
uniform float unClipFar;

void main() {
	vec3 Pos = vec3(unModel * unMesh * vec4(inPos, 1.0f));
	gl_Position = unView * vec4(Pos, 1.0f);
	ouFog = sqrt(min(length(unViewPos - Pos) / unClipFar,1.0f));
}

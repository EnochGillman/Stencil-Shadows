#version 460 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

out vec3 ouPos;
out vec3 ouNormal;
out vec2 ouTexCoord;
out float ouFog;

uniform mat4 unMesh;
uniform mat4 unModel;
uniform mat4 unView;
uniform vec3 unViewPos;
uniform float unClipNear;
uniform float unClipFar;

void main() {
	ouPos = vec3(unModel * unMesh * vec4(inPos, 1.0f));
	gl_Position = unView * vec4(ouPos, 1.0f);
	ouNormal = normalize(vec3(mat4(mat3(unModel * unMesh)) * vec4(inNormal, 1.0f)));
	ouTexCoord = inTexCoord;
	ouFog = sqrt(min(length(unViewPos - ouPos) / unClipFar,1.0f));
}

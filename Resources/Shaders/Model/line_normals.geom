#version 460 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in DATA {
	vec3 Pos;
	vec3 Normal;
	vec2 TexCoord;
	float Fog;
} dataIn[];
out float ouFog;

uniform mat4 unView;

void main() {
	ouFog = dataIn[0].Fog;
	gl_Position = unView * vec4(dataIn[0].Pos, 1.0f);
	EmitVertex();
	gl_Position = unView * vec4(dataIn[0].Pos + dataIn[0].Normal, 1.0f);
	EmitVertex();
	EndPrimitive();

	ouFog = dataIn[1].Fog;
	gl_Position = unView * vec4(dataIn[1].Pos, 1.0f);
	EmitVertex();
	gl_Position = unView * vec4(dataIn[1].Pos + dataIn[1].Normal, 1.0f);
	EmitVertex();
	EndPrimitive();

	ouFog = dataIn[2].Fog;
	gl_Position = unView * vec4(dataIn[2].Pos, 1.0f);
	EmitVertex();
	gl_Position = unView * vec4(dataIn[2].Pos + dataIn[2].Normal, 1.0f);
	EmitVertex();
	EndPrimitive();

}

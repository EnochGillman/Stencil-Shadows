#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in DATA {
	vec3 Pos;
	vec3 Normal;
	vec2 TexCoord;
	float Fog;
} dataIn[];
out DATA {
	vec3 Pos;
	vec3 Normal;
	vec2 TexCoord;
	float Fog;
} ou;

uniform mat4 unView;

void main() {
	gl_Position = unView * vec4(dataIn[0].Pos, 1.0f);
	ou.Pos = dataIn[0].Pos;
	ou.Normal = dataIn[0].Normal;
	ou.TexCoord = dataIn[0].TexCoord;
	ou.Fog = dataIn[0].Fog;
	EmitVertex();

	gl_Position = unView * vec4(dataIn[1].Pos, 1.0f);
	ou.Pos = dataIn[1].Pos;
	ou.Normal = dataIn[1].Normal;
	ou.TexCoord = dataIn[1].TexCoord;
	ou.Fog = dataIn[1].Fog;
	EmitVertex();

	gl_Position = unView * vec4(dataIn[2].Pos, 1.0f);
	ou.Pos = dataIn[2].Pos;
	ou.Normal = dataIn[2].Normal;
	ou.TexCoord = dataIn[2].TexCoord;
	ou.Fog = dataIn[2].Fog;
	EmitVertex();

	EndPrimitive();
}

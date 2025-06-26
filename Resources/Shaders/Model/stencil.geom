#version 460 core

layout (triangles_adjacency) in;
layout (triangle_strip, max_vertices = 18) out;

in DATA {
	vec3 Pos;
	vec3 Normal;
	vec2 TexCoord;
} dataIn[];

uniform mat4 unView;
uniform vec3 unViewPos;
uniform vec4 unLightPos;
float EPSILON;

float length2(vec3 x) {
	return dot(x, x);
}

void edgeDir(vec3 a, vec3 b, vec3 lightDir) {
	gl_Position = unView * vec4(a - lightDir * EPSILON, 1.0f);
	EmitVertex();
	gl_Position = unView * vec4(a - lightDir * 1000.0f, 1.0f);
	EmitVertex();
	gl_Position = unView * vec4(b - lightDir * EPSILON, 1.0f);
	EmitVertex();
	gl_Position = unView * vec4(b - lightDir * 1000.0f, 1.0f);
	EmitVertex();

	EndPrimitive();
}
void edge(vec3 a, vec3 b, vec3 lightDirA, vec3 lightDirB) {
	gl_Position = unView * vec4(a - lightDirA * EPSILON, 1.0f);
	EmitVertex();
	gl_Position = unView * vec4(a - lightDirA * 1000.0f, 1.0f);
	EmitVertex();
	gl_Position = unView * vec4(b - lightDirB * EPSILON, 1.0f);
	EmitVertex();
	gl_Position = unView * vec4(b - lightDirB * 1000.0f, 1.0f);
	EmitVertex();

	EndPrimitive();
}

bool facingLightDir(vec3 a, vec3 b, vec3 c, vec3 lightDir) {
	vec3 n = cross(b - a, c - a);
	return dot(n, lightDir) > 0.0f;
// 		dot(n, lightDir - a) > 0.0f
// 		|| dot(n, lightDir - b) > 0.0f
// 		|| dot(n, lightDir - c) > 0.0f;
}
bool facingLight(vec3 a, vec3 b, vec3 c, vec3 lightDirA, vec3 lightDirB, vec3 lightDirC) {
	vec3 n = cross(b - a, c - a);
// 	return dot(n, lightDirA+lightDirB+lightDirC) > 0.0f;
	return
		dot(n, lightDirA) > 0.0f
		|| dot(n, lightDirB) > 0.0f
		|| dot(n, lightDirC) > 0.0f;
}

void main() {
	EPSILON = 0.5f;
	if(unLightPos.w == 0.0f) {
		vec3 lightDir = vec3(unLightPos);
		if(facingLightDir(dataIn[0].Pos, dataIn[2].Pos, dataIn[4].Pos, lightDir)) {
			for(int i = 0; i < 6; i += 2) {
				if(!facingLightDir(dataIn[i].Pos, dataIn[i+1].Pos, dataIn[(i+2)%6].Pos, lightDir)) {
					edgeDir(dataIn[i].Pos, dataIn[(i+2)%6].Pos, lightDir);
				}
			}

			gl_Position = unView * vec4(dataIn[0].Pos - lightDir * EPSILON, 1.0f);
			EmitVertex();
			gl_Position = unView * vec4(dataIn[2].Pos - lightDir * EPSILON, 1.0f);
			EmitVertex();
			gl_Position = unView * vec4(dataIn[4].Pos - lightDir * EPSILON, 1.0f);
			EmitVertex();

			EndPrimitive();
			gl_Position = unView * vec4(dataIn[0].Pos - lightDir * 1000.0f, 1.0f);
			EmitVertex();
			gl_Position = unView * vec4(dataIn[4].Pos - lightDir * 1000.0f, 1.0f);
			EmitVertex();
			gl_Position = unView * vec4(dataIn[2].Pos - lightDir * 1000.0f, 1.0f);
			EmitVertex();

			EndPrimitive();
		}
	} else {
		vec3 lightDir = vec3(unLightPos);
		vec3 dirs[] = vec3[6](
			normalize(lightDir - dataIn[0].Pos),
			normalize(lightDir - dataIn[1].Pos),
			normalize(lightDir - dataIn[2].Pos),
			normalize(lightDir - dataIn[3].Pos),
			normalize(lightDir - dataIn[4].Pos),
			normalize(lightDir - dataIn[5].Pos)
		);
		if(facingLight(dataIn[0].Pos, dataIn[2].Pos, dataIn[4].Pos, dirs[0], dirs[2], dirs[4])) {
			for(int i = 0; i < 6; i += 2) {
				if(!facingLight(
					dataIn[i].Pos, dataIn[i+1].Pos, dataIn[(i+2)%6].Pos,
					dirs[i], dirs[i+1], dirs[(i+2)%6])
				) {
					edge(dataIn[i].Pos, dataIn[(i+2)%6].Pos, dirs[i], dirs[(i+2)%6]);
				}
			}

			gl_Position = unView * vec4(dataIn[0].Pos - dirs[0] * EPSILON, 1.0f);
			EmitVertex();
			gl_Position = unView * vec4(dataIn[2].Pos - dirs[2] * EPSILON, 1.0f);
			EmitVertex();
			gl_Position = unView * vec4(dataIn[4].Pos - dirs[4] * EPSILON, 1.0f);
			EmitVertex();

			EndPrimitive();
			gl_Position = unView * vec4(dataIn[0].Pos - dirs[0] * 1000.0f, 1.0f);
			EmitVertex();
			gl_Position = unView * vec4(dataIn[4].Pos - dirs[4] * 1000.0f, 1.0f);
			EmitVertex();
			gl_Position = unView * vec4(dataIn[2].Pos - dirs[2] * 1000.0f, 1.0f);
			EmitVertex();

			EndPrimitive();
		}
	}

}

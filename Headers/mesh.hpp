#ifndef MESH_HPP
#define MESH_HPP

#include "texture.hpp"

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texcoord;
};

class Mesh {
	//ids for buffers
	GLuint VBO, VAO, EBO;
	void setupMesh();
	void generateAdjacency();
	bool sameEdge(GLuint a, GLuint b, GLuint c, GLuint d);
	bool compareVerts(GLuint a, GLuint b);
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<GLuint> adjIndices;

	glm::mat4 trans;

	Mesh(std::vector<Vertex> v, std::vector<GLuint> i, glm::mat4 t);
	void use(Shader* shader);
};

class Line {
	static Shader shaderG;	//global
	static Shader shaderS;	//screen

	GLuint VBO, VAO;
	std::vector<glm::vec3> vertices;
	glm::vec3 color;
	bool screen;

public:
	Line(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, bool s = false);
	static void init(glm::mat4* view, glm::mat4* screen);
	void use();
};

#endif

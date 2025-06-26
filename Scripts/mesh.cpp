#include "mesh.hpp"

Mesh::Mesh(std::vector<Vertex> v, std::vector<GLuint> i, glm::mat4 t)
: vertices(v), indices(i), trans(t) {
	setupMesh();
}

bool Mesh::compareVerts(GLuint a, GLuint b) {
	// return length2(vertices[a].pos - vertices[b].pos) < 0.0001f;
	return vertices[a].pos.x == vertices[b].pos.x && vertices[a].pos.y == vertices[b].pos.y && vertices[a].pos.z == vertices[b].pos.z;
}

// bool Mesh::compareVerts(GLuint a, glm::vec3& b) {
// 	// return length2(vertices[a].pos - vertices[b].pos) < 0.0001f;
// 	return vertices[a].pos.x == b.pos.x && vertices[a].pos.y == b.pos.y && vertices[a].pos.z == b.pos.z;
// }

bool Mesh::sameEdge(GLuint a, GLuint b, GLuint c, GLuint d) {
	return (compareVerts(a, c) && compareVerts(b, d)) || (compareVerts(a, d) && compareVerts(b, c));
}

void Mesh::generateAdjacency() {
	unsigned int cntr = 0;
	for(size_t i = 0; i < indices.size(); ++i) {
		adjIndices.push_back(indices[i]);
		adjIndices.push_back(-1);
	}
	bool found = false;
	std::cout << "  min: " << adjIndices.size() << "  max: " << adjIndices.size()*adjIndices.size() << std::endl;
	for(size_t i = 0; i < adjIndices.size(); i += 6) {
		++cntr;
		for(size_t p = 0; p < 6; p += 2) {
			for(size_t j = i+6; j < adjIndices.size(); j += 6) {
				++cntr;
				for(size_t q = 0; q < 6; q += 2) {
					if(
						sameEdge(adjIndices[i+p], adjIndices[i+(p+2)%6], adjIndices[j+q], adjIndices[j+(q+2)%6])
					) {
						adjIndices[i+(p+1)%6] = adjIndices[j+(q+4)%6];
						adjIndices[j+(q+1)%6] = adjIndices[i+(p+4)%6];
						found = true;
						break;
					}
				}
				if(found) {
					found = false;
					break;
				}
			}
		}
		if(adjIndices[i+1] == (unsigned int)(-1)) {
			adjIndices[i+1] = adjIndices[i];
		}
		if(adjIndices[i+3] == (unsigned int)(-1)) {
			adjIndices[i+3] = adjIndices[i+2];
		}
		if(adjIndices[i+5] == (unsigned int)(-1)) {
			adjIndices[i+5] = adjIndices[i+4];
		}
	}
	std::cout << "  iterations: " << cntr << std::endl;
}

// void Mesh::generateAdjacency() {
// 	// cache edges for fast discovery later
// 	std::map<std::tuple<float, float, float, float, float, float>, std::vector<GLuint>> cache;
//
// 	unsigned int cntr = 0;
// 	float* tempVecA;
// 	float* tempVecB;
// 	std::vector<GLuint>* tempVector;
// 	for(size_t i = 0; i < indices.size(); i += 3) {
// 		// put vanilla adjacencies into the adjacency list
// 		adjIndices.push_back(indices[i]);
// 		adjIndices.push_back(-1);
// 		adjIndices.push_back(indices[i+1]);
// 		adjIndices.push_back(-1);
// 		adjIndices.push_back(indices[i+2]);
// 		adjIndices.push_back(-1);
//
// 		// pair edges with their adjacent vertex's index
// 		tempVecA = &vertices[indices[i]].pos[0];
// 		tempVecB = &vertices[indices[i+1]].pos[0];
// 		if(cache.find({
// 			tempVecA[0], tempVecA[1], tempVecA[2], tempVecB[0], tempVecB[1], tempVecB[2]
// 		}) == cache.end()) {
// 			cache[ {
// 				tempVecA[0], tempVecA[1], tempVecA[2], tempVecB[0], tempVecB[1], tempVecB[2]
// 			} ] = {};
// 		}
// 		cache[ {
// 			tempVecA[0], tempVecA[1], tempVecA[2], tempVecB[0], tempVecB[1], tempVecB[2]
// 		} ].push_back(i+2);
//
// 		tempVecA = &vertices[indices[i+1]].pos[0];
// 		tempVecB = &vertices[indices[i+2]].pos[0];
// 		if(cache.find({
// 			tempVecA[0], tempVecA[1], tempVecA[2], tempVecB[0], tempVecB[1], tempVecB[2]
// 		}) == cache.end()) {
// 			cache[ {
// 				tempVecA[0], tempVecA[1], tempVecA[2], tempVecB[0], tempVecB[1], tempVecB[2]
// 			} ] = {};
// 		}
// 		cache[ {
// 			tempVecA[0], tempVecA[1], tempVecA[2], tempVecB[0], tempVecB[1], tempVecB[2]
// 		} ].push_back(i);
//
// 		tempVecA = &vertices[indices[i+2]].pos[0];
// 		tempVecB = &vertices[indices[i]].pos[0];
// 		if(cache.find({
// 			tempVecA[0], tempVecA[1], tempVecA[2], tempVecB[0], tempVecB[1], tempVecB[2]
// 		}) == cache.end()) {
// 			cache[ {
// 				tempVecA[0], tempVecA[1], tempVecA[2], tempVecB[0], tempVecB[1], tempVecB[2]
// 			} ] = {};
// 		}
// 		cache[ {
// 			tempVecA[0], tempVecA[1], tempVecA[2], tempVecB[0], tempVecB[1], tempVecB[2]
// 		} ].push_back(i+1);
// 	}
// 	// bool found = false;
// 	std::cout << "  min: " << adjIndices.size() << "  max: " << adjIndices.size()*adjIndices.size() << std::endl;
// 	for(size_t i = 0; i < adjIndices.size(); i += 6) {
// 		++cntr;
// 		// causes valgrind crash for some reason
// 		if(i%6000 == 999*6) {
// 			std::cout << (float(int(i)) / adjIndices.size() * 100) << "% done" << std::endl;
// 		}
// 		for(size_t p = 0; p < 6; p += 2) {
// 			tempVecA = &vertices[adjIndices[i+p]].pos[0];
// 			tempVecB = &vertices[adjIndices[i+(p+2)%6]].pos[0];
// 			// try finding an edge pointing a certain way
// 			if(cache.find( {tempVecA[0], tempVecA[1], tempVecA[2], tempVecB[0], tempVecB[1], tempVecB[2]} ) != cache.end()) {
// 				tempVector = &cache[ {tempVecA[0], tempVecA[1], tempVecA[2], tempVecB[0], tempVecB[1], tempVecB[2]} ];
// 				// std::cout << (tempVector->size()) << std::endl;
// 				for(GLuint q : *tempVector) {
// 					if(q == i+(p+4)%6) {
// 						// std::cout << "damn1" << std::endl;
// 					} else {
// 						// std::cout << "gucci1" << std::endl;
// 						adjIndices[i+(p+1)%6] = adjIndices[q];
// 					}
// 				}
// 				// adjIndices[i+(p+1)%6] = cache[ {tempVecA[0], tempVecA[1], tempVecA[2], tempVecB[0], tempVecB[1], tempVecB[2]} ];
// 			}
//
// 			// try finding an edge pointing the other way
// 			if(cache.find( {tempVecB[0], tempVecB[1], tempVecB[2], tempVecA[0], tempVecA[1], tempVecA[2]} ) != cache.end()) {
// 				tempVector = &cache[ {tempVecB[0], tempVecB[1], tempVecB[2], tempVecA[0], tempVecA[1], tempVecA[2]} ];
// 				// std::cout << (tempVector->size()) << std::endl;
// 				for(GLuint q : *tempVector) {
// 					if(q == i+(p+4)%6) {
// 						// std::cout << "damn2" << std::endl;
// 					} else {
// 						// std::cout << "gucci2" << std::endl;
// 						adjIndices[i+(p+1)%6] = adjIndices[q];
// 					}
// 				}
// 				// adjIndices[i+(p+1)%6] = cache[ {tempVecB[0], tempVecB[1], tempVecB[2], tempVecA[0], tempVecA[1], tempVecA[2]} ];
// 			}
// 		}
// 		if(adjIndices[i+1] == (unsigned int)(-1)) {
// 			adjIndices[i+1] = adjIndices[i];
// 		}
// 		if(adjIndices[i+3] == (unsigned int)(-1)) {
// 			adjIndices[i+3] = adjIndices[i+2];
// 		}
// 		if(adjIndices[i+5] == (unsigned int)(-1)) {
// 			adjIndices[i+5] = adjIndices[i+4];
// 		}
// 	}
// 	std::cout << "  iterations: " << cntr << std::endl;
// }

void Mesh::setupMesh() {
	//BUFFER SETUP
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//vertex buffer type is GL_ARRAY_BUFFER, we can bind VBO to GL's array buffer
	glBindVertexArray(VAO);
	//copy VBO's data into GL's memory
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	//get the EBO
	generateAdjacency();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(adjIndices[0]) * adjIndices.size(), &adjIndices[0], GL_STATIC_DRAW);
	//set attribute pointers for the VBO
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));

	glBindVertexArray(0);
}

void Mesh::use(Shader* shader) {
	glUniformMatrix4fv(
		glGetUniformLocation(shader->ID, "unMesh"),
		1,
		GL_FALSE,
		glm::value_ptr(trans)
	);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES_ADJACENCY, adjIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Shader Line::shaderG;
Shader Line::shaderS;

Line::Line(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, bool s)
: color(c), screen(s) {
	vertices = std::vector<glm::vec3> {a, b};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glBindVertexArray(0);
}

void Line::init(glm::mat4* view, glm::mat4* screen) {
	shaderG.set(
		"Resources/Shaders/Model/debug_line.vert",
		"Resources/Shaders/Model/debug_line.frag"
	);
	shaderG.add_uniform4m("unView", view);
	shaderS.set(
		"Resources/Shaders/Model/debug_line.vert",
		"Resources/Shaders/Model/debug_line.frag"
	);
	shaderS.add_uniform4m("unView", screen);
}
void Line::use() {
	if(screen) {
		shaderS.use(); glUniform3fv(glGetUniformLocation(shaderS.ID, "unColor"), 1, &color[0]);
	} else {
		shaderG.use(); glUniform3fv(glGetUniformLocation(shaderG.ID, "unColor"), 1, &color[0]);
	}

	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
}

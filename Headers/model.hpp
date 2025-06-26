#ifndef MODEL_HPP
#define MODEL_HPP

#include "json/json.hpp"

#include "mesh.hpp"

class Model {
	std::vector<Mesh> meshes;
	std::vector<std::string> textures;
	std::string directory;

	void loadModel(std::string path);
	std::vector<float> getFloats(std::string& text, nlohmann::json& JSON, unsigned int o);
	std::vector<GLuint> getIndices(std::string& text, nlohmann::json& JSON, unsigned int o);

	std::vector<glm::vec2> lumpVEC2(const std::vector<float>& src);
	std::vector<glm::vec3> lumpVEC3(const std::vector<float>& src);
	std::vector<glm::vec4> lumpVEC4(const std::vector<float>& src);
	std::vector<Vertex> makeVertices(
		const std::vector<glm::vec3>& positions,
		const std::vector<glm::vec3>& normals,
		const std::vector<glm::vec2>& texCoords
	);

	void loadMesh(
		std::string& text,
		nlohmann::json& JSON,
		glm::mat4 trans,
		unsigned int o
	);
	void traverseNode(
		std::string& text,
		nlohmann::json& JSON,
		unsigned int nextNode,
		glm::mat4 trans
	);
public:
	glm::mat4 model {1.0f};

	Model(std::string path);
	void use(Shader* shader);
};

#endif

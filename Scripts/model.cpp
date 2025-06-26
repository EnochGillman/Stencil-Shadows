#include "model.hpp"

Model::Model(std::string path) {
	std::cout << "loading model from " << path << std::endl;
	loadModel(path);
}

std::vector<float> Model::getFloats(std::string& text, nlohmann::json& JSON, unsigned int o) {

	std::vector<float> data;

	//get vertex data
	int bV = JSON["accessors"][o].value("bufferView", 1);
	int bO = JSON["accessors"][o].value("byteOffset", 0);
	int c = JSON["accessors"][o]["count"];
	std::string t = JSON["accessors"][o]["type"];

	nlohmann::json bufferView = JSON["bufferViews"][bV];
	int byteOffset = bufferView.value("byteOffset", 0);

	int dataType = 0;
	if(t == "SCALAR") {dataType = 1;}
	else if(t == "VEC2") {dataType = 2;}
	else if(t == "VEC3") {dataType = 3;}
	else if(t == "VEC4") {dataType = 4;}
	else {std::cout << "invalid bufferView type! must be SCALAR or VEC2/3/4" << std::endl;}

	if(0 < dataType) {
		float* begData = (float*)(&text[0] + bO + byteOffset);
		float* endData = begData + c * dataType;

		for(float* i = begData; i < endData; ++i) {
			data.push_back(*i);
		}
	}

	return data;
}

std::vector<GLuint> Model::getIndices(std::string& text, nlohmann::json& JSON, unsigned int o) {

	std::vector<GLuint> data;

	//get vertex data
	int bV = JSON["accessors"][o].value("bufferView", 1);
	int bO = JSON["accessors"][o].value("byteOffset", 0);
	int c = JSON["accessors"][o]["count"];
	int t = JSON["accessors"][o]["componentType"];

	nlohmann::json bufferView = JSON["bufferViews"][bV];
	int byteOffset = bufferView.value("byteOffset", 0);

	if(t == 5125) {
		unsigned int* begData = (unsigned int*)(&text[0] + bO + byteOffset);
		unsigned int* endData = begData + c;

		for(unsigned int* i = begData; i < endData; ++i) {
			data.push_back(*i);
		}
	} else if(t == 5123) {
		unsigned short* begData = (unsigned short*)(&text[0] + bO + byteOffset);
		unsigned short* endData = begData + c;

		for(unsigned short* i = begData; i < endData; ++i) {
			data.push_back(*i);
		}
	} else if(t == 5122) {
		short* begData = (short*)(&text[0] + bO + byteOffset);
		short* endData = begData + c;

		for(short* i = begData; i < endData; ++i) {
			data.push_back(*i);
		}
	}

	return data;
}

void Model::loadModel(std::string path) {
	//get json data from gltf file
	std::string text = get_file_contents(path);
	nlohmann::json JSON = nlohmann::json::parse(text);

	//get binary data from the model
	path = path.substr(0, path.find_last_of('/') + 1);
	text = get_file_contents(path + std::string(JSON["buffers"][0]["uri"]));
	for(nlohmann::json i : JSON["scenes"]) {
		for(unsigned int j : i["nodes"]) {
			traverseNode(text, JSON, j, glm::mat4(1.0f));
		}
	}
	for(nlohmann::json& i : JSON["images"]) {
		textures.push_back(XSTRING(SOURCE_ROOT) + path + correctWhitespace(i["uri"]));
	}

}

std::vector<glm::vec2> Model::lumpVEC2(const std::vector<float>& src) {
	std::vector<glm::vec2> ret;
	for(size_t i = 0; i < src.size(); i += 2) {
		ret.push_back({src[i], src[i+1]});
	}
	return ret;
}

std::vector<glm::vec3> Model::lumpVEC3(const std::vector<float>& src) {
	std::vector<glm::vec3> ret;
	for(size_t i = 0; i < src.size(); i += 3) {
		ret.push_back({src[i], src[i+1], src[i+2]});
	}
	return ret;
}

std::vector<glm::vec4> Model::lumpVEC4(const std::vector<float>& src) {
	std::vector<glm::vec4> ret;
	for(size_t i = 0; i < src.size(); i += 4) {
		ret.push_back({src[i], src[i+1], src[i+2], src[i+3]});
	}
	return ret;
}

std::vector<Vertex> Model::makeVertices(
	const std::vector<glm::vec3>& positions,
	const std::vector<glm::vec3>& normals,
	const std::vector<glm::vec2>& texCoords
) {
	std::vector<Vertex> ret;
	for(size_t i = 0; i < positions.size(); ++i) {
		ret.push_back({positions[i], normals[i], texCoords[i]});
	}
	return ret;
}

void Model::loadMesh(
	std::string& text,
	nlohmann::json& JSON,
	glm::mat4 trans,
	unsigned int o
) {
	std::vector<Vertex> vertices {
		makeVertices(
			lumpVEC3(
				getFloats(
					text,
					JSON,
					(unsigned int)(
						JSON["meshes"][o]["primitives"][0]["attributes"]["POSITION"]
					)
				)
			),
			lumpVEC3(
				getFloats(
					text,
					JSON,
					(unsigned int)(
						JSON["meshes"][o]["primitives"][0]["attributes"]["NORMAL"]
					)
				)
			),
			lumpVEC2(
				getFloats(
					text,
					JSON,
					(unsigned int)(
						JSON["meshes"][o]["primitives"][0]["attributes"]["TEXCOORD_0"]
					)
				)
			)
		)
	};
	std::vector<GLuint> indices {
		getIndices(
			text,
			JSON,
			(unsigned int)(
				JSON["meshes"][o]["primitives"][0]["indices"]
			)
		)
	};

	meshes.push_back({vertices, indices, trans});
}

void Model::traverseNode(
	std::string& text,
	nlohmann::json& JSON,
	unsigned int nextNode,
	glm::mat4 trans
) {
	nlohmann::json node = JSON["nodes"][nextNode];
	if(node.find("translation") != node.end()) {
		//here, I was facing a weird valgrind bug when converting from string to int
		trans = glm::translate(
			trans,
			 glm::vec3(
				node["translation"][0].is_number_float()
				? node["translation"][0].get<float>()
				: node["translation"][0].get<int>(),
				node["translation"][1].is_number_float()
				? node["translation"][1].get<float>()
				: node["translation"][1].get<int>(),
				node["translation"][2].is_number_float()
				? node["translation"][2].get<float>()
				: node["translation"][2].get<int>()
			 )
		);
	}
	if(node.find("rotation") != node.end()) {
		trans =
			trans * glm::mat4_cast(
				glm::quat(
					node["rotation"][3].is_number_float()
					? node["rotation"][3].get<float>()
					: node["rotation"][3].get<int>(),
					node["rotation"][0].is_number_float()
					? node["rotation"][0].get<float>()
					: node["rotation"][0].get<int>(),
					node["rotation"][1].is_number_float()
					? node["rotation"][1].get<float>()
					: node["rotation"][1].get<int>(),
					node["rotation"][2].is_number_float()
					? node["rotation"][2].get<float>()
					: node["rotation"][2].get<int>()
				)
			);
	}
	if(node.find("scale") != node.end()) {
		trans = glm::scale(
			trans,
		     glm::vec3(
				node["scale"][0].is_number_float()
				? node["scale"][0].get<float>()
				: node["scale"][0].get<int>(),
				node["scale"][1].is_number_float()
				? node["scale"][1].get<float>()
				: node["scale"][1].get<int>(),
				node["scale"][2].is_number_float()
				? node["scale"][2].get<float>()
				: node["scale"][2].get<int>()
		     )
		);
	}
	if(node.find("mesh") != node.end()) {

		loadMesh(text, JSON, trans, node["mesh"]);
	}
	if(node.find("children") != node.end()) {
		for(size_t i = 0; i < node["children"].size(); ++i) {
			traverseNode(text, JSON, node["children"][i], trans);
		}
	}
}

void Model::use(Shader* shader) {
	shader->use();

	unsigned int diffuses = 0;
	unsigned int speculars = 0;
	Texture* currTex;
	std::string name;

	for(unsigned int i = 0; i < textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);

		currTex = Texture::at(textures[i]);
		if(currTex->type == 'd') {
			name = "unMat.diff" + std::to_string(++diffuses);
		} else if(currTex->type == 's') {
			name = "unMat.spec" + std::to_string(++speculars);
		}

		glActiveTexture(GL_TEXTURE0 + i);
		glUniform1i(
			glGetUniformLocation(shader->ID, name.c_str()), i
		);
		glBindTexture(GL_TEXTURE_2D, currTex->id);
	}

	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "unModel"), 1, GL_FALSE, glm::value_ptr(model));
	for(size_t i = 0; i < meshes.size(); ++i) {
		meshes[i].use(shader);
	}
}

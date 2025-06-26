#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <unordered_map>
#include <utility>

#include "shader.hpp"

class Texture {
	static std::unordered_map<std::string, Texture> textures;
public:
	Texture(const std::string name);
	static Texture* at(const std::string name);

	GLuint id;
	char type;		//d: diffuse, s: specular
	int width, height, nrChannels;
};

#endif

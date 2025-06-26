#include "texture.hpp"

std::unordered_map<std::string, Texture> Texture::textures;

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

Texture::Texture(const std::string name) {
	unsigned char* data = stbi_load(
		name.c_str(),
		&width,
		&height,
		&nrChannels,
		0
	);

	if(data) {
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//use r if you have a 3D texture1 ;)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);


		if(nrChannels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		} else if(nrChannels == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		} else if(nrChannels == 1) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
		} else {
			std::cout << "Texture::Texture: could not assign default channel count" << std::endl;
		}
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	} else {
		std::cout << "could not load texture " << name << std::endl;
	}

	if(name.substr(name.find_last_of('/')).find("Roughness") != std::string::npos) {
		type = 's';
	} else {
		type = 'd';
	}
}

Texture* Texture::at(const std::string name) {
	if(textures.find(name) == textures.end()) {
		textures.insert({name, {name}});
	}

	return &textures.at(name);
}

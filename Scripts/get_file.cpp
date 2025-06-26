#include "get_file.hpp"

std::string get_file_contents(std::string filename) {
	std::ifstream in(XSTRING(SOURCE_ROOT) + filename, std::ios::binary);
	if(in) {
		in.seekg(0, std::ios::end);
		size_t size = in.tellg();
		std::string content;
		content.resize(size);
		in.seekg(0, std::ios::beg);
		in.read(&content[0], size);
		in.close();
		return content;
	}
	std::stringstream errmsg;
	errmsg << "shaderClass.cpp: get_file_contents() got messed up\n" << "attempted loading \"" << (XSTRING(SOURCE_ROOT) + filename) << "\"";
	throw std::invalid_argument(errmsg.str());
}

std::string correctWhitespace(std::string s) {
	return std::regex_replace(s, std::regex("%20"), " ");
}

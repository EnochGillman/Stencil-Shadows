#ifndef GET_FILE_H
#define GET_FILE_H

#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

#define STRING(x) #x
#define XSTRING(x) STRING(x)

std::string get_file_contents(std::string filename);
std::string correctWhitespace(std::string s);

#endif

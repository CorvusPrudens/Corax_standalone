
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>
#include <fstream>

std::string file_string(std::string filename)
{
  std::ifstream stream(filename);
  std::stringstream buffer;
  buffer << stream.rdbuf();
  return buffer.str();
}

#endif // UTILS_H
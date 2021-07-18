#include "utils.h"

string file_string(string filename)
{
  std::ifstream stream(filename);
  std::stringstream buffer;
  buffer << stream.rdbuf();
  return buffer.str();
}

void LineData::add(int line_in, int line_out, string file)
{
  int file_ref;
  if (f2idx.count(file) == 0)
  {
    file_ref = files.size();
    f2idx[file] = file_ref;
    files.push_back(file);
  }
  else
  {
    file_ref = f2idx[file];
  }

  LineReference data{line_in, line_out, file_ref};
  lines.push_back(data);
}

string ProcessedCode::getMacro(string name)
{
  if (macros.count(name) == 0)
  {
    throw 1;
  }
  return macros[name];
}

void ProcessedCode::addMacro(string name, string text)
{
  if (macros.count(name) > 0)
  {
    throw 2;
  }
  macros[name] = text;
}

void ProcessedCode::removeMacro(string name)
{
  // actually, this should just silently pass
  if (macros.count(name) == 0)
  {
    return;
  }
  macros.erase(name);
}

void ProcessedCode::addLine(int line_in, string file, string line)
{
  code += line;
  lines.add(line_in, current_line++, file);
}
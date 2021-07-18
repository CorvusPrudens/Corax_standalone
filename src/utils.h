
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>
#include <fstream>

#include <vector>
#include <unordered_map>

using std::string;

string file_string(string filename)
{
  std::ifstream stream(filename);
  std::stringstream buffer;
  buffer << stream.rdbuf();
  return buffer.str();
}

class Macro {

  public:
    Macro() {}
    ~Macro() {}

  private:
    string text;
};

// This will permit us to maintain a reference
// to the original file's line in the output
class LineData {

  public:

    struct LineReference {
      int line_in;
      int line_out;
      int file_in;
    };

    void add(int line_in, int line_out, string file);
    int getLine(int line_out) { return lines[line_out].line_in; }
    string getFile(int line_out) { return files[lines[line_out].file_in]; }
    int length() { return lines.size(); }

  private:
    std::unordered_map<string, int> f2idx;
    std::vector<string> files;
    std::vector<LineReference> lines;
};

// TODO -- add something to allow us to associate
// ranges of code that were previously macros so
// error reporting can be a bit more helpful
struct ProcessedCode {
  int current_line = 0;
  LineData lines;
  std::unordered_map<string, string> macros;
  string code = "";

  string getMacro(string name);
  void addMacro(string name, string text);
  void removeMacro(string name);
  void addLine(int line_in, string file, string line);
};

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

#endif // UTILS_H
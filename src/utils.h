
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>
#include <fstream>

#include <vector>
#include <unordered_map>

using std::string;

string file_string(string filename);

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

#endif // UTILS_H
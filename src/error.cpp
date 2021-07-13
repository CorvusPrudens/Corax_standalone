
#include <iostream>
#include <fstream>
#include <string>

#include "error.h"
#include "colors.h"

using std::string;

string get_line(string filename, int line)
{
  std::ifstream file(filename);
  int i = 0;
  char c;
  while (i < line) {
    file.read(&c, 1);
    if (file.eof())
    {
      return "???";
    }
    if (c == '\n') i++;
  }
  string out;
  std::getline(file, out);

  return out;
}

void Error::AddError(string message, int line, string file, int code, bool fatal)
{
  item e {message, line, file, code};
  errors.push_back(e);
  if (fatal || errors.size() > max_errors)
    Report();
}

void Error::AddWarning(string message, int line, string file, int code)
{
  item w {message, line, file, code};
  warnings.push_back(w);
}

void Error::Report()
{
  int num_errors = errors.size();
  int num_warnings = warnings.size();

  if (num_errors > 0)
    PrintErrors();
  
  if (num_warnings > 0)
    PrintWarnings();

  string ess = num_errors == 1 ? "s, " : ", ";
  std::cout << num_errors << Colors::Red << " error" << ess << Colors::Stop;
  
  ess = num_warnings == 1 ? "s" : "";
  std::cout << num_warnings << Colors::Blue << " warning" << ess << Colors::Stop;

  if (num_errors > 0)
  {
    std::cout << ", exiting...\n";
    exit(1);
  }
  std::cout << "\n";
}

void Error::PrintErrors()
{
  for (auto& e : errors)
  {
    if (e.file != "-1" && e.line != -1)
    {
      std::cout << Colors::Red << "Error " << Colors::Stop;
      std::cout << "in file " << Colors::Green << "\"" << e.file << "\" " << Colors::Stop;
      std::cout << "at line " << e.line << ":\n";

      string line = get_line(e.file, e.line);
      std::cout << Colors::Purple << line << Colors::Stop;
      std::cout << Colors::Green << "  -> " << Colors::Stop << e.message << "\n";
    }
    else
    {
      std::cout << Colors::Red << "Error: \n" << Colors::Stop;
      std::cout << Colors::Green << "  -> " << Colors::Stop << e.message << "\n";
    }
  }
}

void Error::PrintWarnings()
{
  for (auto& w : warnings)
  {
    if (w.file != "-1" && w.line != -1)
    {
      std::cout << Colors::Yellow << "Warning " << Colors::Stop;
      std::cout << "in file " << Colors::Green << "\"" << w.file << "\" " << Colors::Stop;
      std::cout << "at line " << w.line << ":\n";

      string line = get_line(w.file, w.line);
      std::cout << Colors::Purple << line << Colors::Stop;
      std::cout << Colors::Green << "  -> " << Colors::Stop << w.message << "\n";
    }
    else
    {
      std::cout << Colors::Red << "Error: \n" << Colors::Stop;
      std::cout << Colors::Green << "  -> " << Colors::Stop << w.message << "\n";
    }
  }
}
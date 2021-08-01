
#include <iostream>
#include <sstream>
#include <filesystem>

#include "CLI.hpp"
#include "precompiler.h"
#include "compiler.h"
#include "error.h"

using std::filesystem::path;

Compiler comp;
Error err;

int main(int argc, const char* argv[])
{

  CLI::App app{"Compiler for the Corvus flavor of C"};

  std::string filename;
  app.add_option("file", filename, "File name")
    ->required()
    ->check(CLI::ExistingFile);
  bool graph = false;
  app.add_flag("--graph", graph, "Enable function call graphing");

  CLI11_PARSE(app, argc, argv);

  path infile = filename;

  ProcessedCode code;

  Precompiler pre(&code, infile, &err);

  pre.Process();

  int prevnew = 0;
  int curLine = 0;
  // if (!graph)
  // {
  //   std::cout << curLine + 1 << ", " << code.lines.getLine(curLine) << ": ";
  //   curLine++;
  //   for (int i = 0; i < code.code.length(); i++)
  //   {
  //     if (code.code[i] == '\n')
  //     {
  //       std::cout << code.code.substr(prevnew, i + 1 - prevnew);
  //       prevnew = i + 1;
  //       std::cout << curLine + 1 << ", " << code.lines.getLine(curLine) << ": ";
  //       curLine++;
  //     }
  //   }
    
  // }
  // std::cout << code.code;

  // comp.EnableGraph(graph);
  comp.Process(&code, &err);

  // std::cout << prepped;

  // std::ifstream stream2;
  // std::stringstream buffer;
  // stream2.open(filename);
  // buffer << stream2.rdbuf();

  // std::cout << buffer.str();

  // stream2.open(filename);
  // ANTLRInputStream postinput(stream2);
  // comp.Process(&postinput);

  return 0;
}
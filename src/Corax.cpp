
#include <iostream>

#include "CLI.hpp"
#include "antlr4-runtime.h"
#include "precompiler.h"
#include "compiler.h"

PreprocessListener pre;
CompilerListener comp;

int main(int argc, const char* argv[])
{

  CLI::App app{"Compiler for the Corvus flavor of C"};

  std::string filename;
  app.add_option("file", filename, "File name")
    ->required()
    ->check(CLI::ExistingFile);

  CLI11_PARSE(app, argc, argv);

  std::ifstream stream1;
  stream1.open(filename);
  ANTLRInputStream preinput(stream1);
  pre.Process(&preinput);

  std::ifstream stream2;
  stream2.open(filename);
  ANTLRInputStream postinput(stream2);
  comp.Process(&postinput);

  return 0;
}
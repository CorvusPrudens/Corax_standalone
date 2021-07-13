
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <stdio.h>

#include "precompiler.h"
#include "sar.h"
#include "error.h"
#include "utils.h"

using std::string;

string PreprocessListener::Process(string filename, Error* err, bool top, string parent)
{
  Import imp;
  imp.name = filename;
  std::filesystem::path temp_path(parent + '/' + filename);
  imp.parent = temp_path.lexically_normal();
  
  imports.push_back(imp);

  std::cout << imp.parent.native();

  std::ifstream stream(imp.parent.native());
  ANTLRInputStream input(stream);

  PreLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  PreParser parser(&tokens);

  auto tree = parser.parse();
  tree::ParseTreeWalker::DEFAULT.walk(this, tree);

  for (auto import : imports[imports.size() - 1].imports)
  {
    Process(import, err, false, temp_path.parent_path());
  }

  if (top)
  {
    string output = file_string(filename);
    for (auto import : imports)
    {
      for (auto name : import.imports)
      {
        // we'll need to add proper path searching
        string temp_p = import.parent.parent_path();
        std::cout << "import: " << name << ", " << temp_p << "\n";
        string imp_code = file_string(temp_p + "/" + name);
        output = replace(output, "#include +\"" + name + "\"", imp_code);
      }
    }
    return output;
  }
  else
  {
    return "";
  }
}

void PreprocessListener::enterParse(PreParser::ParseContext *ctx)
{
  
}

void PreprocessListener::enterInclude(PreParser::IncludeContext *ctx)
{
  string filename = ctx->STRING()->getText();
  filename = filename.substr(1, filename.length() - 2);

  imports[imports.size() - 1].imports.push_back(filename);
  // string temp = file_string(filename);
}
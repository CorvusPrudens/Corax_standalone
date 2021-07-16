
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

//////////////////////////////
// Import Processor
//////////////////////////////

string ImportListener::Process(string filename, Error* err, bool top, string parent)
{
  Import imp;
  imp.name = filename;
  std::filesystem::path temp_path(parent + '/' + filename);
  imp.parent = temp_path.lexically_normal();
  
  imports.push_back(imp);

  std::cout << imp.parent.native() << "\n";

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
        // std::cout << "import: " << name << ", " << temp_p << "\n";
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

void ImportListener::enterInclude_(PreParser::Include_Context *ctx)
{
  string filename = ctx->STRING()->getText();
  filename = filename.substr(1, filename.length() - 2);

  imports[imports.size() - 1].imports.push_back(filename);
  // string temp = file_string(filename);
}

//////////////////////////////
// Preprocessor
//////////////////////////////

PreprocessListener::Macro 
PreprocessListener::getMacro(string macro_name)
{
  for (auto macro : macros)
  {
    if (macro.name == macro_name)
      return macro;
  }
  Macro empty{"", ""};
  return empty;
}

void PreprocessListener::handleElse(PreParser::Ifdef_Context *ifdef, bool prev_condition)
{
  if (ifdef->else_() != nullptr)
  {
    auto else_ = ifdef->else_();
    if (prev_condition)
    {
      // mark this for removal!
    }
  }
}

void PreprocessListener::handleElse(PreParser::Ifndef_Context *ifdef, bool prev_condition)
{
  if (ifdef->else_() != nullptr)
  {
    auto else_ = ifdef->else_();
    if (prev_condition)
    {
      // mark this for removal!
    }
  }
}

void PreprocessListener::handleElse(PreParser::If_Context *ifdef, bool prev_condition)
{
  if (ifdef->else_() != nullptr)
  {
    auto else_ = ifdef->else_();
    if (prev_condition)
    {
      // mark this for removal!
    }
  }
}

// Process: Collect macros with this processor, skipping any that are 
// contained in a false conditional block, then remove any blocks
// contianed in false conditionals, and finally replace every _NAME_
// using the normal parser
void PreprocessListener::enterDirective(PreParser::DirectiveContext *ctx)
{
  // #define
  if (ctx->define_() != nullptr)
  {
    auto def = ctx->define_();
    Macro macro;
    macro.name = def->NAME()->getText();
    macro.content = "";
    if (def->anything_else() != nullptr)
    {
      macro.content = def->anything_else()->getText();
    }
    macros.push_back(macro);
  }

  if (ctx->ifdef_() != nullptr)
  {
    auto ifdef = ctx->ifdef_();
    bool defined = isDefined(ifdef->NAME()->getText());
    if (defined)
    {
      
    }

    handleElse(ifdef, defined);
  }
}
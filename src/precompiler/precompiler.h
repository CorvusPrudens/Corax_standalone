#pragma once

#include <vector>
#include <filesystem>

#include "antlr4-runtime.h"
#include "PreLexer.h"
#include "PreParser.h"
#include "PreBaseListener.h"

#include "error.h"

using namespace antlr4;
using namespace Pre;
using std::string;

class ImportListener : PreBaseListener {

  public:
    ImportListener() {}
    ~ImportListener() {}

    struct Import {
      string name;
      std::filesystem::path parent;
      std::vector<string> imports;
    };

    string Process(string filename, Error* err, bool top = true, string parent = ".");

  private:
  
    std::vector<Import> imports;
    void enterInclude_(PreParser::Include_Context *ctx) override;
};

class PreprocessListener : PreBaseListener {

  public:
    PreprocessListener() {}
    ~PreprocessListener() {}

    struct Import {
      string name;
      std::filesystem::path parent;
      std::vector<string> imports;
    };

    struct Macro {
      string name;
      string content;
    };

    string Process(string filename, Error* err, bool top = true, string parent = ".");

  private:
  
    std::vector<Macro> macros;
    void enterDirective(PreParser::DirectiveContext *ctx) override;
    Macro getMacro(string macro_name);
    bool isDefined(string macro_name) { return getMacro(macro_name).name != ""; }
    void handleElse(PreParser::If_Context *ifdef, bool prev_condition);
    void handleElse(PreParser::Ifdef_Context *ifdef, bool prev_condition);
    void handleElse(PreParser::Ifndef_Context *ifdef, bool prev_condition);
};
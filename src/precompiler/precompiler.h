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

class PreprocessListener : PreBaseListener {

  public:
    PreprocessListener() {}
    ~PreprocessListener() {}

    struct Import {
      string name;
      std::filesystem::path parent;
      std::vector<string> imports;
    };

    string Process(string filename, Error* err, bool top = true, string parent = ".");

  private:
  
    std::vector<Import> imports;
    void enterParse(PreParser::ParseContext *ctx) override;
    void enterInclude(PreParser::IncludeContext *ctx) override;
};
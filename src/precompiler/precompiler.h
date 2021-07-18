#pragma once

#include <vector>
#include <unordered_map>
#include <filesystem>
#include <regex>

#include "antlr4-runtime.h"
#include "PreLexer.h"
#include "PreParser.h"
#include "PreParserVisitor.h"
#include "PreParserBaseVisitor.h"

#include "error.h"

using namespace antlr4;
using std::string;
using std::filesystem::path;
using std::regex;

// class PreprocessListener : PreBaseListener {

//   public:
//     PreprocessListener() {}
//     ~PreprocessListener() {}

//     struct Import {
//       string name;
//       std::filesystem::path parent;
//       std::vector<string> imports;
//     };

//     struct Macro {
//       string name;
//       string content;
//     };

//     string Process(string filename, Error* err, bool top = true, string parent = ".");

//   private:
  
//     std::vector<Macro> macros;
//     void enterDirective(PreParser::DirectiveContext *ctx) override;
//     Macro getMacro(string macro_name);
//     bool isDefined(string macro_name) { return getMacro(macro_name).name != ""; }
//     void handleElse(PreParser::If_Context *ifdef, bool prev_condition);
//     void handleElse(PreParser::Ifdef_Context *ifdef, bool prev_condition);
//     void handleElse(PreParser::Ifndef_Context *ifdef, bool prev_condition);
// };

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
      int line_out;
      int line_in;
      int file_in;
    };

    void add(int line_in, int line_out, string file);
    int getLine(int line_out) { return lines[line_out].line_in; }
    string getFile(int line_out) { return files[lines[line_out].file_in]; }

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
};

struct Expression {
  bool   isValue;
  double fvalue;
};

class Evaluator : PreParserBaseVisitor {
  public:
    Evaluator(ProcessedCode* code_) {
      code = code_;
    }
    ~Evaluator() {}

    Expression eval(string expression);
    string     expandMacros(string name);

  private:
    tree::ParseTreeProperty<Expression> expr_vals;
    tree::ParseTreeProperty<string> macro_strings;
    ProcessedCode* code;
    
    antlrcpp::Any visitDirName(PreParser::DirNameContext* ctx) override;
    antlrcpp::Any visitDirAny(PreParser::DirAnyContext* ctx) override;
    antlrcpp::Any visitDirHash(PreParser::DirHashContext* ctx) override;
    antlrcpp::Any visitDirDHash(PreParser::DirDHashContext* ctx) override;
};

class Precompiler : PreParserBaseVisitor {

  public:

    Precompiler(ProcessedCode* processed, path file_, Error* err_)
    : evaluator(processed)
    { 
      output = processed;
      file = file_;
      err = err_;
    };

    ~Precompiler() {}

    void Process();

  private:

    CommonTokenStream* tokens;

    static constexpr int max_expansion = 32;
    string expandMacro(string name, int depth=0);
    
    Expression evaluateExpr(string expr);

    void addText(string text, int line_number);

    Evaluator evaluator;
    ProcessedCode* output;
    path file;

    static regex id;
    Error* err;

    antlrcpp::Any visitTopDirective(PreParser::TopDirectiveContext* ctx) override;
    antlrcpp::Any visitIf(PreParser::IfContext* ctx) override;
};
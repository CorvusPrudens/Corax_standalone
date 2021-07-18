
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <regex>

#include "precompiler.h"
#include "sar.h"
#include "error.h"
#include "utils.h"

using std::string;
using std::regex;

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
  if (macros.count(name) == 0)
  {
    throw 3;
  }
  macros.erase(name);
}

// Don't forget to make recursive
string Expander::expand(string text, ProcessedCode* code_)
{
  code = code_;

  string output = "";

  ANTLRInputStream input(text);
  PreLexer lexer(&input);
  CommonTokenStream tokens_(&lexer);
  tokens = &tokens_;
  PreParser parser(tokens);

  PreParser::Anything_exprContext *ctx = parser.anything_expr();
  // tree::ParseTreeWalker::DEFAULT.visit(this, tree);
  if (visitChildren(ctx).isNull())
    std::cout << "NULL\n";
  else
    std::cout << "NOT NULL\n";
  // string expansion = visitChildren(ctx).as<string>();

  return nullptr;
}

antlrcpp::Any Expander::visitAnyName(PreParser::AnyNameContext* ctx)
{
  string text(tokens->getText(ctx));
  std::cout << "NAME: " << text << "\n";
  antlrcpp::Any any(text);
  return any;
}

antlrcpp::Any Expander::visitAnyPass(PreParser::AnyPassContext* ctx)
{
  string text = tokens->getText(ctx);
  std::cout << "ANY: " << text << "\n";
  antlrcpp::Any any(text);

  std::cout << "ANY: " << any.as<string>() << "\n";

  return any;
}

// antlrcpp::Any Evaluator::visitDirName(PreParser::DirNameContext* ctx)
// {
//   string name = ctx->ID()->getText();
//   string expansion;
//   try
//   {
//     string macro = code->getMacro(name);
//     // do recursive thing here
//     string expansion = expandMacros(macro);

//     return expansion;
//   }
//   catch (int e)
//   {
//     return name;
//   }
// }

// antlrcpp::Any Evaluator::visitDirAny(PreParser::DirAnyContext* ctx)
// {
//   return ctx->getText();
// }

// // We'll need to fix this later
// antlrcpp::Any Evaluator::visitDirHash(PreParser::DirHashContext* ctx)
// {
//   return ctx->getText();
// }

// // We'll need to fix this later
// antlrcpp::Any Evaluator::visitDirDHash(PreParser::DirDHashContext* ctx)
// {
//   return ctx->getText();
// }

// string Evaluator::expandMacros(string name)
// {
//   string output = "";

//   ANTLRInputStream input(name);
//   PreLexer lexer(&input);
//   CommonTokenStream tokens(&lexer);
//   PreParser parser(&tokens);

//   PreParser::Anything_elseContext *ctx = parser.anything_else();
//   // tree::ParseTreeWalker::DEFAULT.visit(this, tree);
//   string expansion = visitChildren(ctx).as<string>();

//   return expansion;
// }

// Expression Evaluator::eval(string expression)
// {

//   string expr = expandMacros(expression);

//   std::cout << "Expanded expression: " << expr << "\n";

//   ANTLRInputStream input(expr);
//   PreLexer lexer(&input);
//   CommonTokenStream tokens(&lexer);
//   PreParser parser(&tokens);

//   PreParser::ExpressionContext *ctx = parser.expression();
//   // tree::ParseTreeWalker::DEFAULT.visit(this, tree);
//   visitChildren(ctx);

//   return expr_vals.get(ctx);
// }

regex Precompiler::id("\\b[A-Za-z_][A-Za-z_0-9]*\\b");

void Precompiler::addText(string text, int line_number)
{
  if (text == "\n") {
    output->lines.add(line_number, output->current_line, file.native());
    output->current_line++;
  }
  output->current_line++;
}

// // To avoid building a bunch of trees...
// // Recursively expands macros given current symbol table
// // This will need to be updated for function-like macros!!
// string Precompiler::expandMacro(string sequence, int depth)
// {

//   if (depth > max_expansion)
//   {
//     // TODO -- add an error message
//     // simple debug for now
//     std::cout << "you suck at this";
//     return sequence;
//   }

//   string input  = sequence;
//   string output = sequence.substr();
//   string previous_token = "";

//   auto b = std::sregex_iterator(input.begin(), input.end(), id);
//   auto e = std::sregex_iterator();
//   for (auto iter = b; b != e; ++b)
//   {
//     std::smatch match = *iter;
//     try {
//       string matched = match.str();
//       string replacement = getMacro(matched);
//       replacement = expandMacro(replacement, depth + 1);
//       output = replace(output, matched, replacement);
//     } catch (int e) {
//       // means matched identifier wasn't a macro
//     }
//   }

//   return output;
// }

// Expression Precompiler::evaluateExpr(string expr)
// {
//   string expanded = expandMacro(expr);


// }

antlrcpp::Any Precompiler::visitTopDirective(PreParser::TopDirectiveContext* ctx)
{
  string data = tokens->getText(ctx->directive());
  std::cout << "Directive: " << data << "\n";
  visitChildren(ctx);
  return nullptr;
}

antlrcpp::Any Precompiler::visitIf(PreParser::IfContext* ctx)
{
  // return visitChildren(ctx);
  string expr = tokens->getText(ctx->if_()->anything_expr());

  std::cout << "If: " << expr << "\n";
  // Evaluator eval(output);
  Expander expand;

  string out = expand.expand(expr, output);

  return nullptr;
}


void Precompiler::Process()
{

  std::ifstream stream;
  stream.open(file.native());
  ANTLRInputStream input(stream);
  PreLexer lexer(&input);
  CommonTokenStream tokens_(&lexer);
  tokens = &tokens_;
  // CommonTokenStream tokens(&lexer);
  PreParser parser(&tokens_);

  tree::ParseTree *tree = parser.parse();
  // tree::ParseTreeWalker::DEFAULT.visit(this, tree);
  visit(tree);
  
}
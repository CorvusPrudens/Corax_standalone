
#include <iostream>
#include <stdio.h>

#include "compiler.h"

void CompilerListener::Process(ANTLRInputStream* stream)
{
  PostLexer lexer(stream);
  CommonTokenStream tokens(&lexer);
  PostParser parser(&tokens);

  parse = &parser;

  tree::ParseTree *tree = parser.parse();
  tree::ParseTreeWalker::DEFAULT.walk(this, tree);
}

void CompilerListener::enterParse(PostParser::ParseContext *ctx)
{
  
}

void CompilerListener::enterVariable_init(PostParser::Variable_initContext *ctx)
{
  auto def = ctx->variable_def();
  auto dec = ctx->variable_dec();

  if (def != nullptr)
  {
    std::cout << def->type()->NAME()->getText() << "\n";
    std::cout << def->NAME()->getText() << "\n";
    if (def->assignment()->single() != nullptr)
      std::cout << def->assignment()->single()->expression()->getText() << "\n";
    else
      std::cout << def->assignment()->list()->getText() << "\n";
  }
    
  if (dec != nullptr)
  {
    std::cout << dec->type()->NAME()->getText() << "\n";
    std::cout << dec->NAME()->getText() << "\n";
  }
    
}
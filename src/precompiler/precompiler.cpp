
#include <iostream>
#include <stdio.h>

#include "precompiler.h"

void PreListener::Process(ANTLRInputStream* stream)
{
  Cp_preLexer lexer(stream);
  CommonTokenStream tokens(&lexer);
  Cp_preParser parser(&tokens);

  tree::ParseTree *tree = parser.parse();
  tree::ParseTreeWalker::DEFAULT.walk(this, tree);
}

void PreListener::enterParse(Cp_preParser::ParseContext *ctx)
{
  printf("HELLO WORLDY PRE");
}
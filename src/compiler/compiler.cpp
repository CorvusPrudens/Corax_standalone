
#include <iostream>
#include <stdio.h>

#include "compiler.h"

void PostListener::Process(ANTLRInputStream* stream)
{
  Cp_postLexer lexer(stream);
  CommonTokenStream tokens(&lexer);
  Cp_postParser parser(&tokens);

  tree::ParseTree *tree = parser.parse();
  tree::ParseTreeWalker::DEFAULT.walk(this, tree);
}

void PostListener::enterParse(Cp_postParser::ParseContext *ctx)
{
  printf("HELLO WORLDY");
}
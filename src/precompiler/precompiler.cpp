
#include <iostream>
#include <stdio.h>

#include "precompiler.h"

void PreprocessListener::Process(ANTLRInputStream* stream)
{
  PreLexer lexer(stream);
  CommonTokenStream tokens(&lexer);
  PreParser parser(&tokens);

  tree::ParseTree *tree = parser.parse();
  tree::ParseTreeWalker::DEFAULT.walk(this, tree);
}

void PreprocessListener::enterParse(PreParser::ParseContext *ctx)
{
  
}
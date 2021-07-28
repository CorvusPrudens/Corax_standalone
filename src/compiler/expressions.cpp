#include <iostream>
#include <stdio.h>

#include "compiler.h"

using antlrcpp::Any;

////////////////////////////////////////////////
// EXPRESSIONS
////////////////////////////////////////////////

Any Compiler::visitCall(PostParser::CallContext* ctx)
{
  if (graphing) graph.Addc(ctx->expr_primary()->getText());
  return visitChildren(ctx);
}


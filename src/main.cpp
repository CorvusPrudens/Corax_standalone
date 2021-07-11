
#include <iostream>

#include "antlr4-runtime.h"
#include "precompiler.h"
#include "compiler.h"

PreListener pre;
PostListener post;

int main(int argc, const char* argv[])
{
  std::ifstream stream;
  stream.open(argv[1]);
  ANTLRInputStream input(stream);

  pre.Process(&input);
  post.Process(&input);

  return 0;
}
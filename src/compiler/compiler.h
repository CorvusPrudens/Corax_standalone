#pragma once

#include <iostream>
#include <vector>
#include "antlr4-runtime.h"
#include "PostLexer.h"
#include "PostParser.h"
#include "PostBaseListener.h"

#include "variable.h"

using namespace antlr4;
using namespace Post;

class CompilerListener : PostBaseListener {

  public:
    CompilerListener() {}
    ~CompilerListener() {}

    void Process(ANTLRInputStream* stream);

    void enterParse(PostParser::ParseContext* ctx) override;
    void enterVariable_init(PostParser::Variable_initContext* ctx) override;

  private:

    std::vector<Variable> globals;
    PostParser* parse;

};
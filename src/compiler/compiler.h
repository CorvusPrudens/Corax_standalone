
#include <iostream>
#include "antlr4-runtime.h"
#include "Cp_postLexer.h"
#include "Cp_postParser.h"
#include "Cp_postBaseListener.h"

using namespace antlr4;
using namespace Post;

class PostListener : Cp_postBaseListener {

  public:
    PostListener() {}
    ~PostListener() {}

    void Process(ANTLRInputStream* stream);

  private:

    void enterParse(Cp_postParser::ParseContext *ctx) override;

};
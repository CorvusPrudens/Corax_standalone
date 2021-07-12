
#include "antlr4-runtime.h"
#include "PreLexer.h"
#include "PreParser.h"
#include "PreBaseListener.h"

using namespace antlr4;
using namespace Pre;

class PreprocessListener : PreBaseListener {

  public:
    PreprocessListener() {}
    ~PreprocessListener() {}

    void Process(ANTLRInputStream* stream);

  private:
  
    void enterParse(PreParser::ParseContext *ctx) override;

};
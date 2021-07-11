
#include "antlr4-runtime.h"
#include "Cp_preLexer.h"
#include "Cp_preParser.h"
#include "Cp_preBaseListener.h"

using namespace antlr4;
using namespace Pre;

class PreListener : Cp_preBaseListener {

  public:
    PreListener() {}
    ~PreListener() {}

    void Process(ANTLRInputStream* stream);

  private:
  
    void enterParse(Cp_preParser::ParseContext *ctx) override;

};
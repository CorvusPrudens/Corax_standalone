#include <string>

class Operand {

  public:
    Operand(std::string val, bool lit = false) {
      value = val;
      literal = lit;
    }
    ~Operand() {}
    
    std::string value;
    bool literal;
    
};
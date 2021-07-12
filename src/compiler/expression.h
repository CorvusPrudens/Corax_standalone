
#include <string>
#include <vector>
#include "operand.h"

class Expression {

  public:
    Expression() {}
    ~Expression() {}

    // operator precedence and subgroups need to be figured out
    std::vector<Operand> operands;
    std::vector<std::string> operators;
    
};
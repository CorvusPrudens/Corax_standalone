
#include <vector>
#include "variable.h"

class Scope {

  public:
    Scope() {}
    ~Scope() {}

    
    std::vector<Variable> variables;
    std::vector<Variable> statics;
  
  private:

};


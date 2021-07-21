
#include "type.h"
#include <string>

using std::string;

class Variable {

  public:

    enum Initializer {
      NONE = 0,
      CONST,
      VARIABLE,
    };

    Variable(Type t, string n) {
      name = n;
      type = t;
      init = Initializer::NONE;
    }
    ~Variable() {}

    // A string represents the const initialization so we can
    // convert it later to its numeric representation without a 
    // loss of information
    void setInitializer(string s)
    {
      constInit = s;
      init = Initializer::CONST;
    }

    void setInitializer(Variable v)
    {
      assignment = v;
      init = Initializer::VARIABLE;
    }

    bool isConst()
    {
      return type.isConst();
    }

    string name;
    Type type;
    string constInit;
    Variable assignment;

  private:

};
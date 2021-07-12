
#include <string>

enum TYPE {
  INT = 0,
  STRUCT
};

class Variable {

  public:
    Variable(TYPE t, std::string n, std::string ts, std::string a = "") {
      type = t;
      name = n;
      type_string = ts;
      assignment = a;
      initialized = a != "";
    }
    ~Variable() {}

    TYPE type;
    bool initialized;

    std::string name;
    std::string type_string;
    std::string assignment;

  private:

};
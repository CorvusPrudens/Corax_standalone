
#include <string>

class Variable {

  public:
    Variable(std::string t, std::string n, std::string a = "") {
      name = n;
      type = t;
      assignment = a;
      initialized = a != "";
    }
    ~Variable() {}

    bool initialized;

    std::string name;
    std::string type;
    std::string assignment;

  private:

};
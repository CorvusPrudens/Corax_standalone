
#include <string>
#include <vector>

using std::string;

class Identifier {

  public:
    enum IdType {
      Function = 0,
      Struct,
      FunctionPointer,
      Pointer,
      Variable,
      Array,
    };

    Identifier(string n, IdType t) {
      name = n;
      type = t;
    }
    ~Identifier() {}

    void setInt(int i) { ivalue = i; }
    void setFloat(float f) { fvalue = f; }
    void setDataType(string s) { dataType = s; }
    void setReturnType(string s) 

    // This might be expanded on later
    IdType   getType() { return type; }
    string getName() { return name; }
    int    getInt() { return ivalue; }
    float  getFloat() { return fvalue; }
    string getDataType() { return dataType; }
    string getReturnType() { return returnType; }

    std::vector<int> getArray() { return itable; }

    // Perhaps a bit wasteful, we'll come up with a better solution later i guess
    IdType type;
    string name;
    string dataType;
    string returnType;

    std::vector<int> itable;
    int ivalue;
    double fvalue;

    std::vector<Identifier> args;


};
#include "variable.h"
#include "function.h"
#include "struct.h"
#include "union.h"
#include "array.h"

#include <string>
#include <vector>

using std::string;

class Identifier {

  public:
    enum IdType {
      Function = 0,
      Struct,
      Union,
      // FunctionPointer,
      // Pointer,
      Variable,
      Array,
      // Typedef,
    };

    Identifier(string n, IdType t) {
      name = n;
      type = t;
    }
    ~Identifier() {}

    bool operator==(Identifier other)
    {
      switch(type)
      {
        case IdType::Function:
          return (other.type == IdType::Function && function_ == other.function_);
        case IdType::Struct:
          return (other.type == IdType::Struct && struct_ == other.struct_);
        case IdType::Union:
          return (other.type == IdType::Union && union_ == other.union_);
        case IdType::Variable:
          return (other.type == IdType::Variable && variable_ == other.variable_);
        case IdType::Array:
          return (other.type == IdType::Array && array_ == other.array_);
        default:
          return false;
      }
    }

    IdType type;
    string name;

    Function function_;
    Struct   struct_;
    Union    union_;
    Variable variable_;
    Array    array_;

    // Type dataType;
    // Type returnType;

    // std::vector<int> itable;
    // int ivalue;
    // double fvalue;

    // std::vector<Identifier> args;
};
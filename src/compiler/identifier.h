#include "type.h"

#include <string>
#include <vector>

using std::string;

class Identifier {

  public:
    enum IdType {
      FUNCTION = 0,
      STRUCT,
      UNION,
      // FunctionPointer,
      // Pointer,
      VARIABLE,
      ARRAY,
      // Typedef,
    };

    // Identifier(string n, IdType t) {
    //   name = n;
    //   type = t;
    // }
    Identifier() {}
    ~Identifier() {}

    bool operator==(Identifier other)
    {
      switch(type)
      {
        case IdType::FUNCTION:
          return (other.type == IdType::FUNCTION && function_ == other.function_);
        case IdType::STRUCT:
          return (other.type == IdType::STRUCT && struct_ == other.struct_);
        case IdType::UNION:
          return (other.type == IdType::UNION && union_ == other.union_);
        case IdType::VARIABLE:
          return (other.type == IdType::VARIABLE && variable_ == other.variable_);
        case IdType::ARRAY:
          return (other.type == IdType::ARRAY && array_ == other.array_);
        default:
          return false;
      }
    }

    Identifier copy()
    {
      Identifier newid;
      newid.type = type;
      newid.name = name;
      switch(type)
      {
        case IdType::FUNCTION:
          newid.function_ = function_;
          break;
        case IdType::STRUCT:
          newid.struct_ = struct_;
          break;
        case IdType::UNION:
          newid.union_ = union_;
          break;
        case IdType::VARIABLE:
          newid.variable_ = variable_;
          break;
        case IdType::ARRAY:
          newid.array_ = array_;
          break;
        default:
          newid.variable_ = variable_;
          break;
      }
      return newid;
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

class Struct {

  public:

    // Struct(Type t, string n) {
    //   name = n;
    //   type = t;
    //   initialized = false;
    // }
    Struct() {}
    ~Struct() {}

    bool isConst()
    {
      return type.isConst();
    }

    void setMembers(std::vector<Identifier> m) {
      members = m;
      initialized = true;
    }

    bool operator==(Struct& other) {
      return type == other.type && MatchingVector(members, other.members);
    }

    string name;
    Type type;
    std::vector<Identifier> members;
    bool initialized;

  private:

};

class Union {

  public:

    // Union(Type t, string n) {
    //   name = n;
    //   type = t;
    //   initialized = false;
    // }
    Union() {}
    ~Union() {}

    bool isConst()
    {
      return type.isConst();
    }

    void setMembers(std::vector<Identifier> m) {
      members = m;
      initialized = true;
    }

    bool operator==(Union& other) {
      return type == other.type && MatchingVector(members, other.members);
    }

    string name;
    Type type;
    std::vector<Identifier> members;
    bool initialized;

  private:

};

class Variable {

  public:

    enum Initializer {
      NONE = 0,
      CONST,
      VARIABLE,
    };

    // Variable(Type t, string n) {
    //   name = n;
    //   type = t;
    //   init = Initializer::NONE;
    // }
    Variable() {}
    ~Variable() {}

    // A string represents the const initializer so we can
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

    bool operator==(Variable& other) {
      switch (init)
      {
        case Initializer::NONE:
          return other.init == init && other.type == type;
        case Initializer::CONST:
          return other.init == init && other.type == type && other.constInit == constInit;
        case Initializer::VARIABLE:
          return other.init == init && other.type == type && other.assignment == assignment;
        default:
          return false;
      }
    }

    string name;
    Type type;
    string constInit;
    Variable assignment;
    Initializer init;

  private:

};

class Array {

  public:

    // size of -1 indicates variably sized!
    // Array(Type t, string n, int s) {
    //   name = n;
    //   type = t;
    //   size = s;
    //   initialized = false;
    // }
    Array() {}
    ~Array() {}

    bool isConst()
    {
      return type.isConst();
    }

    void setMembers(std::vector<string> i) {
      initializers = i;
      initialized = true;
    }

    bool operator==(Array& other) {
      // TODO -- matching vector func is not appropriate for array!!
      return type == other.type && MatchingVector(initializers, other.initializers);
    }

    string name;
    Type type;
    std::vector<int> dimensions;
    std::vector<string> initializers;
    // TODO -- figure out way to represent either literals or variables
    // std::vector<Identifier*> dimensions;
    bool initialized;

  private:

};

class Function {
  public:

    // Function(Type t, Type rt, string n) {
    //   name = n;
    //   type = t;
    //   returnType = rt;
    // }
    Function() {}
    ~Function() {}

    // A string represents the const initializer so we can
    // convert it later to its numeric representation without a 
    // loss of information
    void setArgs(std::vector<Identifier> i)
    {
      args = i;
    }

    bool isConst()
    {
      return type.isConst();
    }

    bool operator==(Function& other) {
      // TODO -- matching vector func is not appropriate for function!!
      return type == other.type && returnType == other.returnType && MatchingVector(args, other.args);
    }

    string name;
    Type type;
    Type returnType;
    std::vector<Identifier> args;
    // string constInit;
    // Variable assignment;
}
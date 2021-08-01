#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include "antlr4-runtime.h"
#include "type.h"

#include <string>
#include <vector>

using std::string;
using antlr4::ParserRuleContext;
using std::vector;

class Instruction; // forward declaration

class FuncComp {

  public:
    FuncComp() {}
    ~FuncComp() {}
    FuncComp(const FuncComp& other) { instructions = other.instructions; }

    void add(Instruction inst);

    /** Removes any trivially unnecessary temporary variables
     * 
     */
    void clean();
    // not sure what to do about variables from
    // the global scope
    // void setTree()

    string to_string();

  private:
    // vector<SymbolTable> symbol_tree;
    vector<Instruction> instructions;
};

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
    Identifier() {
      name = "";
      type = IdType::VARIABLE;
      initialized = false;
    }
    ~Identifier() {}

    Identifier(const Identifier& other)
    {
      type = other.type;
      name = other.name;
      // We can optimize this later
      dataType = other.dataType;
      returnType = other.returnType;
      initialized = other.initialized;
      members = other.members;
      initializers = other.initializers;
      function = other.function;
    }

    bool operator==(Identifier& other)
    {
      return equal(other);
    }

    bool operator!=(Identifier& other)
    {
      return !equal(other);
    }

    Identifier copy()
    {
      Identifier newid;
      newid.type = type;
      newid.name = name;
      // We can optimize this later
      newid.dataType = dataType;
      newid.returnType = returnType;
      newid.initialized = initialized;
      newid.members = members;
      newid.initializers = initializers;
      newid.function = function;
      return newid;
    }

    IdType type;
    string name;

    Type dataType;
    Type returnType;
    bool initialized;

    // For struct / union / function
    std::vector<Identifier> members;

    // For function
    FuncComp function;

    // For array
    std::vector<string> initializers;

    private:

      bool equal(Identifier& other)
      {
        switch(type)
        {
          case IdType::FUNCTION:
            {
              bool equal = other.type == IdType::FUNCTION;
              equal = equal && other.dataType == dataType;
              equal = equal && other.returnType == returnType;
              // TODO -- this needs expansion:
              // it needs to check if the parameter order and 
              // types are the same, not names etc.
              if (members.size() != other.members.size()) return false;
              for (int i = 0; i < members.size(); i++)
              {
                if (members[i].dataType != other.members[i].dataType)
                  return false;
              }
              return equal;
            }
          case IdType::STRUCT:
            {
              bool equal = other.type == IdType::STRUCT;
              equal = equal && other.dataType == dataType;
              equal = equal && EqualVectors(other.members, members);
              return equal;
            }
          case IdType::UNION:
            {
              bool equal = other.type == IdType::UNION;
              equal = equal && other.dataType == dataType;
              equal = equal && EqualVectors(other.members, members);
              return equal;
            }
          case IdType::VARIABLE:
            {
              bool equal = other.type == IdType::VARIABLE;
              return equal && other.dataType == dataType && other.name == name;
            }
          case IdType::ARRAY:
            {
              bool equal = other.type == IdType::FUNCTION;
              equal = equal && other.dataType == dataType;
              equal = equal && other.returnType == returnType;
              equal = equal && EqualVectors(other.members, members);
              return equal;
            }
          default:
            return false;
        }
      }
};

class Result {

  public:

    static constexpr size_t buff_size = 16;

    enum Kind {
      VOID = 0,
      ID,
      STANDARD,
      STRUCT, // dunno if we'll use these?
      UNION,
    };

    uint8_t value[buff_size];
    Identifier id;
    Type type;
    Kind kind = Kind::VOID;

    Result() {
      for (int i = 0; i < buff_size; i++)
        value[i] = 0;
    }
    ~Result() {}

    // copy constructor
    Result(const Result& other);

    bool isConst() { return kind != Kind::ID; }

    string to_string();
    void to(Type t);

    void setValue(Identifier& new_id)
    {
      kind = Kind::ID;
      id = new_id;
    }

    void setValue(long double val);
    void setValue(double val);
    void setValue(float val);
    void setValue(unsigned long long val);
    void setValue(long long val);
    void setValue(unsigned long val);
    void setValue(long val);
    void setValue(unsigned val);
    void setValue(int val);
    void setValue(unsigned short val);
    void setValue(short val);
    void setValue(unsigned char val);
    void setValue(signed char val);
    void setValue(char val);

    template<typename T>
    T as()
    {
      if (kind == Kind::ID)
        throw 1;
      if (type == long_double_) {
        long double* orig = (long double*) value;
        T val = (T) *orig;
        return val;
      } else if (type == double_) {
        double* orig = (double*) value;
        T val = (T) *orig;
        return val;
      } else if (type == float_) {
        float* orig = (float*) value;
        T val = (T) *orig;
        return val;
      } else if (type == unsigned_long_long_) {
        unsigned long long* orig = (unsigned long long*) value;
        T val = (T) *orig;
        return val;
      } else if (type == long_long_) {
        long long* orig = (long long*) value;
        T val = (T) *orig;
        return val;
      } else if (type == unsigned_long_) {
        unsigned long* orig = (unsigned long*) value;
        T val = (T) *orig;
        return val;
      } else if (type == long_) {
        long* orig = (long*) value;
        T val = (T) *orig;
        return val;
      } else if (type == unsigned_) {
        unsigned* orig = (unsigned*) value;
        T val = (T) *orig;
        return val;
      } else if (type == int_) {
        int* orig = (int*) value;
        T val = (T) *orig;
        return val;
      } else if (type == unsigned_short_) {
        unsigned short* orig = (unsigned short*) value;
        T val = (T) *orig;
        return val;
      } else if (type == short_) {
        short* orig = (short*) value;
        T val = (T) *orig;
        return val;
      } else if (type == unsigned_char_) {
        // WHOA these can all be auto??
        auto orig = (unsigned char*) value;
        T val = (T) *orig;
        return val;
      } else if (type == signed_char_) {
        auto orig = (signed char*) value;
        T val = (T) *orig;
        return val;
      } else if (type == char_) {
        auto orig = (char*) value;
        T val = (T) *orig;
        return val;
      } else if (type == void_) {
        throw 2;
      }
      throw 3;
    }

    
};

class Instruction {

  public:
    enum Abstr {
      DEREF = 0,
      NOT,
      NEGATE,
      NEGATIVE,
      CONVERT,
      ASSIGN,
      ADD,
      SUB,
      MULT,
      DIV,
      MOD,
      SHIFT_L,
      SHIFT_R,
      BIT_AND,
      BIT_XOR,
      BIT_OR,
      AND,
      OR,
      CMP,
    };

    enum Cond {
      EQUAL,
      NOT_EQUAL,
      GREATER,
      LESS,
      GREATER_EQUAL,
      LESS_EQUAL,
    };

    Instruction(ParserRuleContext* c, Abstr i, Result op1, Identifier& ass);
    Instruction(ParserRuleContext* c, Abstr i, Result op1, Result op2, Identifier& ass);
    Instruction(ParserRuleContext* c, Abstr i, Cond co, Result op1, Result op2, Identifier& ass);
    Instruction(const Instruction& other);
    ~Instruction() {}

    void setCondition(Cond c) { condition = c; }
    string to_string();

    Abstr instr;
    Cond condition;
    Result operand1;
    Result operand2;
    Identifier assignment;
    ParserRuleContext* ctx; // for easy error reporting
    bool single;

};

// class Struct {

//   public:

//     // Struct(Type t, string n) {
//     //   name = n;
//     //   type = t;
//     //   initialized = false;
//     // }
//     Struct() {}
//     ~Struct() {}

//     bool isConst()
//     {
//       return type.isConst();
//     }

//     void setMembers(std::vector<Identifier> m) {
//       members = m;
//       initialized = true;
//     }

//     bool operator==(Struct& other) {
//       return type == other.type && MatchingVector(members, other.members);
//     }

//     string name;
//     Type type;
//     std::vector<Identifier> members;
//     bool initialized;

//   private:

// };

// class Union {

//   public:

//     // Union(Type t, string n) {
//     //   name = n;
//     //   type = t;
//     //   initialized = false;
//     // }
//     Union() {}
//     ~Union() {}

//     bool isConst()
//     {
//       return type.isConst();
//     }

//     void setMembers(std::vector<Identifier> m) {
//       members = m;
//       initialized = true;
//     }

//     bool operator==(Union& other) {
//       return type == other.type && MatchingVector(members, other.members);
//     }

//     string name;
//     Type type;
//     std::vector<Identifier> members;
//     bool initialized;

//   private:

// };

// class Variable {

//   public:

//     enum Initializer {
//       NONE = 0,
//       CONST,
//       VARIABLE,
//     };

//     // Variable(Type t, string n) {
//     //   name = n;
//     //   type = t;
//     //   init = Initializer::NONE;
//     // }
//     Variable() {}
//     ~Variable() {}

//     // A string represents the const initializer so we can
//     // convert it later to its numeric representation without a 
//     // loss of information
//     void setInitializer(string s)
//     {
//       constInit = s;
//       init = Initializer::CONST;
//     }

//     void setInitializer(Variable v)
//     {
//       assignment = v;
//       init = Initializer::VARIABLE;
//     }

//     bool isConst()
//     {
//       return type.isConst();
//     }

//     bool operator==(Variable& other) {
//       switch (init)
//       {
//         case Initializer::NONE:
//           return other.init == init && other.type == type;
//         case Initializer::CONST:
//           return other.init == init && other.type == type && other.constInit == constInit;
//         case Initializer::VARIABLE:
//           return other.init == init && other.type == type && other.assignment == assignment;
//         default:
//           return false;
//       }
//     }

//     string name;
//     Type type;
//     string constInit;
//     Variable assignment;
//     Initializer init;

//   private:

// };

// class Array {

//   public:

//     // size of -1 indicates variably sized!
//     // Array(Type t, string n, int s) {
//     //   name = n;
//     //   type = t;
//     //   size = s;
//     //   initialized = false;
//     // }
//     Array() {}
//     ~Array() {}

//     bool isConst()
//     {
//       return type.isConst();
//     }

//     void setMembers(std::vector<string> i) {
//       initializers = i;
//       initialized = true;
//     }

//     bool operator==(Array& other) {
//       // TODO -- matching vector func is not appropriate for array!!
//       return type == other.type && MatchingVector(initializers, other.initializers);
//     }

//     string name;
//     Type type;
//     std::vector<int> dimensions;
//     std::vector<string> initializers;
//     // TODO -- figure out way to represent either literals or variables
//     // std::vector<Identifier*> dimensions;
//     bool initialized;

//   private:

// };

// class Function {
//   public:

//     // Function(Type t, Type rt, string n) {
//     //   name = n;
//     //   type = t;
//     //   returnType = rt;
//     // }
//     Function() {}
//     ~Function() {}

//     // A string represents the const initializer so we can
//     // convert it later to its numeric representation without a 
//     // loss of information
//     void setArgs(std::vector<Identifier> i)
//     {
//       args = i;
//     }

//     bool isConst()
//     {
//       return type.isConst();
//     }

//     bool operator==(Function& other) {
//       // TODO -- matching vector func is not appropriate for function!!
//       return type == other.type && returnType == other.returnType && MatchingVector(args, other.args);
//     }

//     string name;
//     Type type;
//     Type returnType;
//     std::vector<Identifier> args;
//     // string constInit;
//     // Variable assignment;
// }

#endif
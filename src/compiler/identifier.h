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
class SymbolTable; // forward decl
class Result; // forward decl

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

    Identifier() {
      name = "";
      type = IdType::VARIABLE;
      initialized = false;
      latest = 0;
    }
    ~Identifier() {}

    Identifier(const Identifier& other);

    bool operator==(Identifier& other) { return equal(other); }
    bool operator!=(Identifier& other) { return !equal(other); }
    bool operator==(Identifier* other) { return equal(other); }
    bool operator!=(Identifier* other) { return !equal(other); }

    IdType type;
    string name;

    Type dataType;
    Type returnType;
    SymbolTable* funcTable;
    // for keeping track of variable updates in function
    unsigned int latest; 

    // TODO -- add warnings for operations on uninitialized variables!
    bool initialized;

    // For struct / union / function
    std::vector<Identifier> members;

    // For function
    FuncComp function;

    // For array
    std::vector<Result> initializers;

    private:

      bool equal(Identifier& other);
      bool equal(Identifier* other);
      
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
    Identifier* id;
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
    bool equal(Result& other, bool permissive = false);
    bool operator==(Result& other) { return equal(other); }
    bool operator!=(Result& other) { return !equal(other); }

    string to_string();
    void to(Type t);

    void setValue(Identifier& new_id)
    {
      kind = Kind::ID;
      id = &new_id;
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
    string name();

    Abstr instr;
    Cond condition;
    Result operand1;
    Result operand2;
    Identifier* assignment;
    ParserRuleContext* ctx; // for easy error reporting
    bool single;

};

#endif
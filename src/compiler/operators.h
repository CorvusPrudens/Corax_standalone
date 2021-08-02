
#include "compiler.h"

#define STANDARD_PERFORM \
  using OperatorBase::OperatorBase; \
  void perform(long double v1, long double v2, Result& res) override { operation(v1, v2, res); } \
  void perform(double v1, double v2, Result& res) override { operation(v1, v2, res); } \
  void perform(float v1, float v2, Result& res) override { operation(v1, v2, res); } \
  void perform(unsigned long long v1, unsigned long long v2, Result& res) override { operation(v1, v2, res); } \
  void perform(long long v1, long long v2, Result& res) override { operation(v1, v2, res); } \
  void perform(unsigned long v1, unsigned long v2, Result& res) override { operation(v1, v2, res); } \
  void perform(long v1, long v2, Result& res) override { operation(v1, v2, res); } \
  void perform(unsigned v1, unsigned v2, Result& res) override { operation(v1, v2, res); } \
  void perform(int v1, int v2, Result& res) override { operation(v1, v2, res); } \
  void perform(unsigned short v1, unsigned short v2, Result& res) override { operation(v1, v2, res); } \
  void perform(unsigned char v1, unsigned char v2, Result& res) override { operation(v1, v2, res); } \
  void perform(signed char v1, signed char v2, Result& res) override { operation(v1, v2, res); } \
  void perform(char v1, char v2, Result& res) override { operation(v1, v2, res); }

#define INTEGRAL_PERFORM \
  using OperatorBase::OperatorBase; \
  void perform(unsigned long long v1, unsigned long long v2, Result& res) override { operation(v1, v2, res); } \
  void perform(long long v1, long long v2, Result& res) override { operation(v1, v2, res); } \
  void perform(unsigned long v1, unsigned long v2, Result& res) override { operation(v1, v2, res); } \
  void perform(long v1, long v2, Result& res) override { operation(v1, v2, res); } \
  void perform(unsigned v1, unsigned v2, Result& res) override { operation(v1, v2, res); } \
  void perform(int v1, int v2, Result& res) override { operation(v1, v2, res); } \
  void perform(unsigned short v1, unsigned short v2, Result& res) override { operation(v1, v2, res); } \
  void perform(unsigned char v1, unsigned char v2, Result& res) override { operation(v1, v2, res); } \
  void perform(signed char v1, signed char v2, Result& res) override { operation(v1, v2, res); } \
  void perform(char v1, char v2, Result& res) override { operation(v1, v2, res); }

#define OPERATION(operator) \
  template <typename T> \
  void operation(T v1, T v2, Result& res) \
  { \
    T out = v1 operator v2; \
    res.setValue(out); \
  }

#define OPERATION_LOG(operator) \
  template <typename T> \
  void operation(T v1, T v2, Result& res) \
  { \
    int out = v1 operator v2; \
    res.setValue(out); \
  }

#define INTEGRAL_CHECK \
  bool validType1(Type& t) override { \
    if (t == long_double_ || t == double_ || t == float_) \
      return false; \
    return true; \
  } \
  bool validType2(Type& t) override { \
    if (t == long_double_ || t == double_ || t == float_) \
      return false; \
    return true; \
  }

class Equal : public OperatorBase {

  public:

    STANDARD_PERFORM
    OPERATION_LOG(==)

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::CMP; }
    Instruction::Cond getCond() override { return Instruction::Cond::EQUAL; }
};

class NotEqual : public OperatorBase {

  public:

    STANDARD_PERFORM
    OPERATION_LOG(!=)

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::CMP; }
    Instruction::Cond getCond() override { return Instruction::Cond::NOT_EQUAL; }
};

class Greater : public OperatorBase {

  public:

    STANDARD_PERFORM
    OPERATION_LOG(>)

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::CMP; }
    Instruction::Cond getCond() override { return Instruction::Cond::GREATER; }
};

class Less : public OperatorBase {

  public:

    STANDARD_PERFORM
    OPERATION_LOG(<)

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::CMP; }
    Instruction::Cond getCond() override { return Instruction::Cond::LESS; }
};

class GreaterEqual : public OperatorBase {

  public:

    STANDARD_PERFORM
    OPERATION_LOG(>=)

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::CMP; }
    Instruction::Cond getCond() override { return Instruction::Cond::GREATER_EQUAL; }
};

class LessEqual : public OperatorBase {

  public:

    STANDARD_PERFORM
    OPERATION_LOG(<=)

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::CMP; }
    Instruction::Cond getCond() override { return Instruction::Cond::LESS_EQUAL; }
};

class Shiftl : public OperatorBase {

  public:

    INTEGRAL_PERFORM
    OPERATION(<<)
    INTEGRAL_CHECK

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::SHIFT_L; }
};

class Shiftr : public OperatorBase {

  public:

    INTEGRAL_PERFORM
    OPERATION(>>)
    INTEGRAL_CHECK

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::SHIFT_R; }
};

class BitAnd : public OperatorBase {

  public:

    INTEGRAL_PERFORM
    OPERATION(&)
    INTEGRAL_CHECK

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::BIT_AND; }
};

class BitOr : public OperatorBase {

  public:

    INTEGRAL_PERFORM
    OPERATION(|)
    INTEGRAL_CHECK

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::BIT_OR; }
};

class BitXor : public OperatorBase {

  public:

    INTEGRAL_PERFORM
    OPERATION(^)
    INTEGRAL_CHECK

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::BIT_XOR; }
};

// These actually shouldn't have any conversions!
class And : public OperatorBase {

  public:

    STANDARD_PERFORM
    OPERATION_LOG(&&)

    virtual void perform(Result& op1, Identifier& id, Result& res) override {
      Result rhs;
      rhs.setValue(id);

      // string tempname = "__temp_var_" + std::to_string(table->temp_vars++) + "__";
      // Identifier& ass;
      // ass.name = tempname;
      // ass.dataType.type_specifiers = {"int"};
      Type tt;
      tt.type_specifiers = {"int"};
      Identifier& ass = manageTemps(tt);

      func->function.add(Instruction(ctx, getAbstr(), op1, rhs, ass));
  
      res.setValue(ass);
    }

    virtual void perform(Identifier& id1, Result& op2, Result& res) override {
      Result lhs;
      lhs.setValue(id1);

      // string tempname = "__temp_var_" + std::to_string(table->temp_vars++) + "__";
      // Identifier& ass;
      // ass.name = tempname;
      // ass.dataType.type_specifiers = {"int"};
      Type tt;
      tt.type_specifiers = {"int"};
      Identifier& ass = manageTemps(tt);

      func->function.add(Instruction(ctx, getAbstr(), lhs, op2, ass));
  
      res.setValue(ass);
    }

    virtual void perform(Identifier& id1, Identifier& id2, Result& res) override {
      Result lhs;
      lhs.setValue(id1);
      Result rhs;
      rhs.setValue(id2);

      // string tempname = "__temp_var_" + std::to_string(table->temp_vars++) + "__";
      // Identifier& ass;
      // ass.name = tempname;
      // ass.dataType.type_specifiers = {"int"};
      Type tt;
      tt.type_specifiers = {"int"};
      Identifier& ass = manageTemps(tt);

      func->function.add(Instruction(ctx, getAbstr(), lhs, rhs, ass));
  
      res.setValue(ass);
    }

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::AND; }
};

class Or : public OperatorBase {

  public:

    STANDARD_PERFORM
    OPERATION_LOG(||)

    virtual void perform(Result& op1, Identifier& id, Result& res) override {
      Result rhs;
      rhs.setValue(id);

      // string tempname = "__temp_var_" + std::to_string(table->temp_vars++) + "__";
      // Identifier& ass;
      // ass.name = tempname;
      // ass.dataType.type_specifiers = {"int"};
      Type tt;
      tt.type_specifiers = {"int"};
      Identifier& ass = manageTemps(tt);

      func->function.add(Instruction(ctx, getAbstr(), op1, rhs, ass));
  
      res.setValue(ass);
    }

    virtual void perform(Identifier& id1, Result& op2, Result& res) override {
      Result lhs;
      lhs.setValue(id1);

      // string tempname = "__temp_var_" + std::to_string(table->temp_vars++) + "__";
      // Identifier& ass;
      // ass.name = tempname;
      // ass.dataType.type_specifiers = {"int"};
      Type tt;
      tt.type_specifiers = {"int"};
      Identifier& ass = manageTemps(tt);

      func->function.add(Instruction(ctx, getAbstr(), lhs, op2, ass));
  
      res.setValue(ass);
    }

    virtual void perform(Identifier& id1, Identifier& id2, Result& res) override {
      Result lhs;
      lhs.setValue(id1);
      Result rhs;
      rhs.setValue(id2);

      // string tempname = "__temp_var_" + std::to_string(table->temp_vars++) + "__";
      // Identifier& ass;
      // ass.name = tempname;
      // ass.dataType.type_specifiers = {"int"};
      Type tt;
      tt.type_specifiers = {"int"};
      Identifier& ass = manageTemps(tt);

      func->function.add(Instruction(ctx, getAbstr(), lhs, rhs, ass));
  
      res.setValue(ass);
    }

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::OR; }
};

class Mult : public OperatorBase {

  public:

    STANDARD_PERFORM
    OPERATION(*)

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::MULT; }
};

class Div : public OperatorBase {

  public:

    STANDARD_PERFORM
    OPERATION(/)

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::DIV; }
};

// TODO -- make this work with floats on lhs
class Mod : public OperatorBase {

  public:

    INTEGRAL_PERFORM
    OPERATION(%)

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::MOD; }
};

class Add : public OperatorBase {

  public:

    STANDARD_PERFORM
    OPERATION(+)

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::ADD; }
};

class Sub : public OperatorBase {

  public:

    STANDARD_PERFORM
    OPERATION(-)

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::SUB; }
};

class Negative : public OperatorBase {

  public:

    using OperatorBase::OperatorBase;

    void perform(long double v1, Result& res) override { operation(v1, res); }
    void perform(double v1, Result& res) override { operation(v1, res); }
    void perform(float v1, Result& res) override { operation(v1, res); }
    // void perform(unsigned long long v1, unsigned long long v2, Result& res) override { operation(v1, v2, res); }
    void perform(long long v1, Result& res) override { operation(v1, res); }
    // void perform(unsigned long v1, unsigned long v2, Result& res) override { operation(v1, v2, res); }
    void perform(long v1, Result& res) override { operation(v1, res); }
    // void perform(unsigned v1, unsigned v2, Result& res) override { operation(v1, v2, res); }
    void perform(int v1, Result& res) override { operation(v1, res); }
    // void perform(unsigned short v1, unsigned short v2, Result& res) override { operation(v1, v2, res); }
    // void perform(unsigned char v1, unsigned char v2, Result& res) override { operation(v1, v2, res); }
    void perform(signed char v1, signed char v2, Result& res) override { operation(v1, res); }
    // void perform(char v1, char v2, Result& res) override { operation(v1, v2, res); }

    template <typename T>
    void operation(T v1, Result& res)
    {
      T out = -v1;
      res.setValue(out);
    }

    bool validType1(Type& t) override { 
      if 
      (
        t == unsigned_long_long_ ||
        t == unsigned_long_ ||
        t == unsigned_ ||
        t == unsigned_short_ ||
        t == unsigned_char_
      )
      return false;

      return true;
    }

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::NEGATIVE; }
};

class Not : public OperatorBase {

  public:

    using OperatorBase::OperatorBase;

    // void perform(long double v1, Result& res) override { operation(v1, res); }
    // void perform(double v1, Result& res) override { operation(v1, res); }
    // void perform(float v1, Result& res) override { operation(v1, res); }
    void perform(unsigned long long v1, Result& res) override { operation(v1, res); }
    void perform(long long v1, Result& res) override { operation(v1, res); }
    void perform(unsigned long v1, Result& res) override { operation(v1, res); }
    void perform(long v1, Result& res) override { operation(v1, res); }
    void perform(unsigned v1, Result& res) override { operation(v1, res); }
    void perform(int v1, Result& res) override { operation(v1, res); }
    void perform(unsigned short v1, Result& res) override { operation(v1, res); }
    void perform(unsigned char v1, Result& res) override { operation(v1, res); }
    void perform(signed char v1, Result& res) override { operation(v1, res); }
    void perform(char v1, Result& res) override { operation(v1, res); }

    template <typename T>
    void operation(T v1, Result& res)
    {
      T out = ~v1;
      res.setValue(out);
    }

    bool validType1(Type& t) override { 
      if 
      (
        t == float_ ||
        t == double_ ||
        t == long_double_
      )
      return false;

      return true;
    }

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::NOT; }
};

class Negate : public OperatorBase {

  public:

    using OperatorBase::OperatorBase;

    void perform(long double v1, Result& res) override { operation(v1, res); }
    void perform(double v1, Result& res) override { operation(v1, res); }
    void perform(float v1, Result& res) override { operation(v1, res); }
    void perform(unsigned long long v1, Result& res) override { operation(v1, res); }
    void perform(long long v1, Result& res) override { operation(v1, res); }
    void perform(unsigned long v1, Result& res) override { operation(v1, res); }
    void perform(long v1, Result& res) override { operation(v1, res); }
    void perform(unsigned v1, Result& res) override { operation(v1, res); }
    void perform(int v1, Result& res) override { operation(v1, res); }
    void perform(unsigned short v1, Result& res) override { operation(v1, res); }
    void perform(unsigned char v1, Result& res) override { operation(v1, res); }
    void perform(signed char v1, Result& res) override { operation(v1, res); }
    void perform(char v1, Result& res) override { operation(v1, res); }

    template <typename T>
    void operation(T v1, Result& res)
    {
      int out = !v1;
      res.setValue(out);
    }

    virtual void perform(Identifier& id1, Result& res) override {
      Result lhs;
      lhs.setValue(id1);

      // string tempname = "__temp_var_" + std::to_string(table->temp_vars++) + "__";
      // Identifier& ass;
      // ass.name = tempname;
      // ass.dataType.type_specifiers = {"int"};
      Type tt;
      tt.type_specifiers = {"int"};
      Identifier& ass = manageTemps(tt);

      func->function.add(Instruction(ctx, getAbstr(), lhs, ass));
  
      res.setValue(ass);
    }

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::NEGATE; }
};

class Assign : public OperatorBase {

  public:

    using OperatorBase::OperatorBase;

    void perform(Identifier& id, Result& op2, Result& res) override {
      if (op2.type != id.dataType) {
        op2.to(id.dataType);
      }
      // in some cases this could be optimized out, but for now we'll
      // assume it should be an operation
      // id.assignment = op2;
      func->function.add(Instruction(ctx, getAbstr(), op2, id));
      
      // what is this supposed to be set to??
      res.setValue(0);
    }

    void perform(Identifier& id1, Identifier& id2, Result& res) override {
      if (id2.dataType != id1.dataType) 
      {
        Result temp;
        temp.setValue(id2);
        Result rhs;
        rhs.setValue(id1);
        func->function.add(Instruction(ctx, Instruction::Abstr::CONVERT, temp, rhs, id1));
      }
      else 
      {
        Result temp;
        temp.setValue(id2);
        func->function.add(Instruction(ctx, getAbstr(), temp, id1));
      }
      
      // what is this supposed to be set to??
      res.setValue(0);
    }

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::ASSIGN; }
};

class IncrPost : public OperatorBase {

  public:

    using OperatorBase::OperatorBase;

    void perform(Identifier& id, Result& res) override {
      
      Result op2;
      op2.setValue(1);
      op2.to(id.dataType);

      Result lhs;
      lhs.setValue(id);

      // If any post-expr operation happens on the same identifier,
      // then we have a warning
      for (auto inst : table->postExpr)
      {
        if (*inst.assignment == id) {
          string warnmess = "ambiguous postfix operation order";
          // TODO -- add error option with mutliple nodes so we
          // can avoid referencing the same node twice with this (like if there's three!)
          comp->addRuleWarn(ctx, warnmess);
          comp->addRuleWarn(inst.ctx, warnmess);
        }
      }
      
      table->postExpr.push_back(Instruction(ctx, getAbstr(), lhs, op2, id));
      
      res.setValue(id);
    }

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::ADD; }
};

class DecrPost : public OperatorBase {

  public:

    using OperatorBase::OperatorBase;

    void perform(Identifier& id, Result& res) override {
      
      Result op2;
      op2.setValue(1);
      op2.to(id.dataType);

      Result lhs;
      lhs.setValue(id);

      // If any post-expr operation happens on the same identifier,
      // then we have a warning
      for (auto inst : table->postExpr)
      {
        if (*inst.assignment == id) {
          string warnmess = "ambiguous postfix operation order";
          // TODO -- add error option with mutliple nodes so we
          // can avoid referencing the same node twice with this (like if there's three!)
          comp->addRuleWarn(ctx, warnmess);
          comp->addRuleWarn(inst.ctx, warnmess);
        }
      }
      
      table->postExpr.push_back(Instruction(ctx, getAbstr(), lhs, op2, id));
      
      res.setValue(id);
    }

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::SUB; }
};

class IncrPre : public OperatorBase {

  public:

    using OperatorBase::OperatorBase;

    void perform(Identifier& id, Result& res) override {
      
      Result op2;
      op2.setValue(1);
      op2.to(id.dataType);

      Result lhs;
      lhs.setValue(id);

      // TODO -- come up with way to warn about multiple prefix operations
      // for (auto inst : table->postExpr)
      // {
      //   if (inst.assignment == id) {
      //     string warnmess = "ambiguous postfix operation order";
      //     // TODO -- add error option with mutliple nodes so we
      //     // can avoid referencing the same node twice with this (like if there's three!)
      //     comp->addRuleErr(ctx, warnmess);
      //     comp->addRuleErr(inst.ctx, warnmess);
      //   }
      // }
      
      func->function.add(Instruction(ctx, getAbstr(), lhs, op2, id));
      
      res.setValue(id);
    }

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::ADD; }
};

class DecrPre : public OperatorBase {

  public:

    using OperatorBase::OperatorBase;

    void perform(Identifier& id, Result& res) override {
      
      Result op2;
      op2.setValue(1);
      op2.to(id.dataType);

      Result lhs;
      lhs.setValue(id);

      // TODO -- come up with way to warn about multiple prefix operations
      // for (auto inst : table->postExpr)
      // {
      //   if (inst.assignment == id) {
      //     string warnmess = "ambiguous postfix operation order";
      //     // TODO -- add error option with mutliple nodes so we
      //     // can avoid referencing the same node twice with this (like if there's three!)
      //     comp->addRuleErr(ctx, warnmess);
      //     comp->addRuleErr(inst.ctx, warnmess);
      //   }
      // }
      
      func->function.add(Instruction(ctx, getAbstr(), lhs, op2, id));
      
      res.setValue(id);
    }

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::SUB; }
};
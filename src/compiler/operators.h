
#include "compiler.h"

class Mult : public OperatorBase {

  public:

    using OperatorBase::OperatorBase;

    void perform(long double v1, long double v2, Result& res) override { operation(v1, v2, res); }
    void perform(double v1, double v2, Result& res) override { operation(v1, v2, res); }
    void perform(float v1, float v2, Result& res) override { operation(v1, v2, res); }
    void perform(unsigned long long v1, unsigned long long v2, Result& res) override { operation(v1, v2, res); }
    void perform(long long v1, long long v2, Result& res) override { operation(v1, v2, res); }
    void perform(unsigned long v1, unsigned long v2, Result& res) override { operation(v1, v2, res); }
    void perform(long v1, long v2, Result& res) override { operation(v1, v2, res); }
    void perform(unsigned v1, unsigned v2, Result& res) override { operation(v1, v2, res); }
    void perform(int v1, int v2, Result& res) override { operation(v1, v2, res); }
    void perform(unsigned short v1, unsigned short v2, Result& res) override { operation(v1, v2, res); }
    void perform(unsigned char v1, unsigned char v2, Result& res) override { operation(v1, v2, res); }
    void perform(signed char v1, signed char v2, Result& res) override { operation(v1, v2, res); }
    void perform(char v1, char v2, Result& res) override { operation(v1, v2, res); }

    template <typename T>
    void operation(T v1, T v2, Result& res)
    {
      T out = v1 * v2;
      res.setValue(out);
    }

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::MULT; }
};

class Div : public OperatorBase {

  public:

    using OperatorBase::OperatorBase;

    void perform(long double v1, long double v2, Result& res) override { operation(v1, v2, res); }
    void perform(double v1, double v2, Result& res) override { operation(v1, v2, res); }
    void perform(float v1, float v2, Result& res) override { operation(v1, v2, res); }
    void perform(unsigned long long v1, unsigned long long v2, Result& res) override { operation(v1, v2, res); }
    void perform(long long v1, long long v2, Result& res) override { operation(v1, v2, res); }
    void perform(unsigned long v1, unsigned long v2, Result& res) override { operation(v1, v2, res); }
    void perform(long v1, long v2, Result& res) override { operation(v1, v2, res); }
    void perform(unsigned v1, unsigned v2, Result& res) override { operation(v1, v2, res); }
    void perform(int v1, int v2, Result& res) override { operation(v1, v2, res); }
    void perform(unsigned short v1, unsigned short v2, Result& res) override { operation(v1, v2, res); }
    void perform(unsigned char v1, unsigned char v2, Result& res) override { operation(v1, v2, res); }
    void perform(signed char v1, signed char v2, Result& res) override { operation(v1, v2, res); }
    void perform(char v1, char v2, Result& res) override { operation(v1, v2, res); }

    template <typename T>
    void operation(T v1, T v2, Result& res)
    {
      T out = v1 / v2;
      res.setValue(out);
    }

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::DIV; }
};

// TODO -- make this work with floats on lhs
class Mod : public OperatorBase {

  public:

    using OperatorBase::OperatorBase;

    void perform(unsigned long long v1, unsigned long long v2, Result& res) override { operation(v1, v2, res); }
    void perform(long long v1, long long v2, Result& res) override { operation(v1, v2, res); }
    void perform(unsigned long v1, unsigned long v2, Result& res) override { operation(v1, v2, res); }
    void perform(long v1, long v2, Result& res) override { operation(v1, v2, res); }
    void perform(unsigned v1, unsigned v2, Result& res) override { operation(v1, v2, res); }
    void perform(int v1, int v2, Result& res) override { operation(v1, v2, res); }
    void perform(unsigned short v1, unsigned short v2, Result& res) override { operation(v1, v2, res); }
    void perform(unsigned char v1, unsigned char v2, Result& res) override { operation(v1, v2, res); }
    void perform(signed char v1, signed char v2, Result& res) override { operation(v1, v2, res); }
    void perform(char v1, char v2, Result& res) override { operation(v1, v2, res); }

    template <typename T>
    void operation(T v1, T v2, Result& res)
    {
      T out = v1 % v2;
      res.setValue(out);
    }

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::MOD; }
};

class Add : public OperatorBase {

  public:

    using OperatorBase::OperatorBase;

    void perform(long double v1, long double v2, Result& res) override { operation(v1, v2, res); }
    void perform(double v1, double v2, Result& res) override { operation(v1, v2, res); }
    void perform(float v1, float v2, Result& res) override { operation(v1, v2, res); }
    void perform(unsigned long long v1, unsigned long long v2, Result& res) override { operation(v1, v2, res); }
    void perform(long long v1, long long v2, Result& res) override { operation(v1, v2, res); }
    void perform(unsigned long v1, unsigned long v2, Result& res) override { operation(v1, v2, res); }
    void perform(long v1, long v2, Result& res) override { operation(v1, v2, res); }
    void perform(unsigned v1, unsigned v2, Result& res) override { operation(v1, v2, res); }
    void perform(int v1, int v2, Result& res) override { operation(v1, v2, res); }
    void perform(unsigned short v1, unsigned short v2, Result& res) override { operation(v1, v2, res); }
    void perform(unsigned char v1, unsigned char v2, Result& res) override { operation(v1, v2, res); }
    void perform(signed char v1, signed char v2, Result& res) override { operation(v1, v2, res); }
    void perform(char v1, char v2, Result& res) override { operation(v1, v2, res); }

    template <typename T>
    void operation(T v1, T v2, Result& res)
    {
      T out = v1 + v2;
      res.setValue(out);
    }

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::ADD; }
};

class Sub : public OperatorBase {

  public:

    using OperatorBase::OperatorBase;

    void perform(long double v1, long double v2, Result& res) override { operation(v1, v2, res); }
    void perform(double v1, double v2, Result& res) override { operation(v1, v2, res); }
    void perform(float v1, float v2, Result& res) override { operation(v1, v2, res); }
    void perform(unsigned long long v1, unsigned long long v2, Result& res) override { operation(v1, v2, res); }
    void perform(long long v1, long long v2, Result& res) override { operation(v1, v2, res); }
    void perform(unsigned long v1, unsigned long v2, Result& res) override { operation(v1, v2, res); }
    void perform(long v1, long v2, Result& res) override { operation(v1, v2, res); }
    void perform(unsigned v1, unsigned v2, Result& res) override { operation(v1, v2, res); }
    void perform(int v1, int v2, Result& res) override { operation(v1, v2, res); }
    void perform(unsigned short v1, unsigned short v2, Result& res) override { operation(v1, v2, res); }
    void perform(unsigned char v1, unsigned char v2, Result& res) override { operation(v1, v2, res); }
    void perform(signed char v1, signed char v2, Result& res) override { operation(v1, v2, res); }
    void perform(char v1, char v2, Result& res) override { operation(v1, v2, res); }

    template <typename T>
    void operation(T v1, T v2, Result& res)
    {
      T out = v1 - v2;
      res.setValue(out);
    }

    Instruction::Abstr getAbstr() override { return Instruction::Abstr::SUB; }
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

#include <iostream>
#include "identifier.h"
#include "antlr4-runtime.h"

using antlr4::tree::ParseTree;
using std::cout;
using std::string;

Result::Result(const Result& other)
{
  if (other.kind == Kind::ID)
    id = other.id;
  else
  {
    for (int i = 0; i < buff_size; i++)
      value[i] = other.value[i];
  }
  kind = other.kind;
}

void Result::to(Type t)
{
  if (kind == Kind::ID)
    throw 1;
  if (t == long_double_) {
    auto temp = as<long double>();
    setValue(temp);
  } else if (t == double_) {
    auto temp = as<double>();
    setValue(temp);
  } else if (t == float_) {
    auto temp = as<float>();
    setValue(temp);
  } else if (t == unsigned_long_long_) {
    auto temp = as<unsigned long long>();
    setValue(temp);
  } else if (t == long_long_) {
    auto temp = as<long long>();
    setValue(temp);
  } else if (t == unsigned_long_) {
    auto temp = as<unsigned long>();
    setValue(temp);
  } else if (t == long_) {
    auto temp = as<long>();
    setValue(temp);
  } else if (t == unsigned_) {
    auto temp = as<unsigned>();
    setValue(temp);
  } else if (t == int_) {
    auto temp = as<int>();
    setValue(temp);
  } else if (t == unsigned_short_) {
    auto temp = as<unsigned short>();
    setValue(temp);
  } else if (t == short_) {
    auto temp = as<short>();
    setValue(temp);
  } else if (t == unsigned_char_) {
    auto temp = as<unsigned char>();
    setValue(temp);
  } else if (t == signed_char_) {
    auto temp = as<signed char>();
    setValue(temp);
  } else if (t == char_) {
    auto temp = as<char>();
    setValue(temp);
  } else if (t == void_) {
    throw 2;
  }
}

string Result::to_string()
{
  string s = "";
  if (kind == Kind::ID)
    s = id.name;
  else if (type == long_double_) {
    s = as<long double>();
  } else if (type == double_) {
    s = as<double>();
  } else if (type == float_) {
    s = as<float>();
  } else if (type == unsigned_long_long_) {
    s = as<unsigned long long>();
  } else if (type == long_long_) {
    s = as<long long>();
  } else if (type == unsigned_long_) {
    s = as<unsigned long>();
  } else if (type == long_) {
    s = as<long>();
  } else if (type == unsigned_) {
    s = as<unsigned>();
  } else if (type == int_) {
    s = as<int>();
  } else if (type == unsigned_short_) {
    s = as<unsigned short>();
  } else if (type == short_) {
    s = as<short>();
  } else if (type == unsigned_char_) {
    s = as<unsigned char>();
  } else if (type == signed_char_) {
    s = as<signed char>();
  } else if (type == char_) {
    s = as<char>();
  } else if (type == void_) {
    s = "void";
  } else {
    s = "<error-val>";
  }
  return s + "\n";
}

void Result::setValue(long double val)
{
  kind = Kind::STANDARD;
  type.type_specifiers = {"double", "long"};
  long double* ptr = (long double*) value;
  *ptr = val;
}
void Result::setValue(double val)
{
  kind = Kind::STANDARD;
  type.type_specifiers = {"double"};
  double* ptr = (double*) value;
  *ptr = val;
}
void Result::setValue(float val)
{
  kind = Kind::STANDARD;
  type.type_specifiers = {"float"};
  float* ptr = (float*) value;
  *ptr = val;
}
void Result::setValue(unsigned long long val)
{
  kind = Kind::STANDARD;
  type.type_specifiers = {"long", "long", "unsigned"};
  long long unsigned* ptr = (long long unsigned*) value;
  *ptr = val;
}
void Result::setValue(long long val)
{
  kind = Kind::STANDARD;
  type.type_specifiers = {"long", "long"};
  long long* ptr = (long long*) value;
  *ptr = val;
}
void Result::setValue(unsigned long val)
{
  kind = Kind::STANDARD;
  type.type_specifiers = {"long", "unsigned"};
  unsigned long* ptr = (unsigned long*) value;
  *ptr = val;
}
void Result::setValue(long val)
{
  kind = Kind::STANDARD;
  type.type_specifiers = {"long"};
  long* ptr = (long*) value;
  *ptr = val;
}
void Result::setValue(unsigned val)
{
  kind = Kind::STANDARD;
  type.type_specifiers = {"unsigned"};
  unsigned* ptr = (unsigned*) value;
  *ptr = val;
}
void Result::setValue(int val)
{
  kind = Kind::STANDARD;
  type.type_specifiers = {"int"};
  int* ptr = (int*) value;
  *ptr = val;
}
void Result::setValue(unsigned short val)
{
  kind = Kind::STANDARD;
  type.type_specifiers = {"short", "unsigned"};
  unsigned short* ptr = (unsigned short*) value;
  *ptr = val;
}
void Result::setValue(short val)
{
  kind = Kind::STANDARD;
  type.type_specifiers = {"short"};
  short* ptr = (short*) value;
  *ptr = val;
}
void Result::setValue(unsigned char val)
{
  kind = Kind::STANDARD;
  type.type_specifiers = {"char", "unsigned"};
  unsigned char* ptr = (unsigned char*) value;
  *ptr = val;
}
void Result::setValue(signed char val)
{
  kind = Kind::STANDARD;
  type.type_specifiers = {"char", "signed"};
  signed char* ptr = (signed char*) value;
  *ptr = val;
}
void Result::setValue(char val)
{
  kind = Kind::STANDARD;
  type.type_specifiers = {"char"};
  char* ptr = (char*) value;
  *ptr = val;
}

Instruction::Instruction(ParseTree* c, Abstr i, Result op1, Identifier& ass) {
  ctx = c;
  instr = i;
  operand1 = op1;
  single = true;
  assignment = ass;
}

Instruction::Instruction(ParseTree* c, Abstr i, Result op1, Result op2, Identifier& ass) {
  ctx = c;
  instr = i;
  operand1 = op1;
  operand2 = op2;
  single = false;
  assignment = ass;
}

Instruction::Instruction(const Instruction& other) {
  ctx = other.ctx;
  instr = other.instr;
  operand1 = other.operand1;
  single = other.single;
  condition = other.condition;
  if (!single)
    operand2 = other.operand2;
  assignment = other.assignment;
}

void Instruction::debugPrint() {
  cout << assignment.name << " = ";
  switch (instr) {
    case DEREF:
    {
      if (single)
        cout << "*" << operand1.to_string() << "\n";
      else
        cout << "*(" << operand1.to_string() << " + " << operand2.to_string() << ")\n";
    }
    break;
    case NOT:
    {
      cout << "~" << operand1.to_string() << "\n";
    }
    break;
    case NEGATE:
    {
      cout << "!" << operand1.to_string() << "\n";
    }
    break;
    case CONVERT:
    {
      if (operand1.isConst())
      {
        cout << operand1.to_string() << " to " << assignment.dataType.to_string();
      }
      else
      {
        cout << operand1.to_string() << " (" << operand1.id.dataType.to_string();
        cout << ") to " << assignment.dataType.to_string();
      }
    }
    break;
    case ASSIGN:
    {
      cout << operand1.to_string() << "\n";
    }
    break;
    case ADD:
    {
      cout << operand2.to_string() << " + " << operand2.to_string() << "\n";
    }
    break;
    case SUB:
    {
      cout << operand2.to_string() << " - " << operand2.to_string() << "\n";
    }
    break;
    case MULT:
    {
      cout << operand2.to_string() << " * " << operand2.to_string() << "\n";
    }
    break;
    case DIV:
    {
      cout << operand2.to_string() << " / " << operand2.to_string() << "\n";
    }
    break;
    case MOD:
    {
      cout << operand2.to_string() << " % " << operand2.to_string() << "\n";
    }
    break;
    case SHIFT_L:
    {
      cout << operand2.to_string() << " << " << operand2.to_string() << "\n";
    }
    break;
    case SHIFT_R:
    {
      cout << operand2.to_string() << " >> " << operand2.to_string() << "\n";
    }
    break;
    case AND:
    {
      cout << operand2.to_string() << " & " << operand2.to_string() << "\n";
    }
    break;
    case XOR:
    {
      cout << operand2.to_string() << " ^ " << operand2.to_string() << "\n";
    }
    break;
    case OR:
    {
      cout << operand2.to_string() << " | " << operand2.to_string() << "\n";
    }
    break;
    case CMP:
    {
      switch (condition)
      {
        case EQUAL:
          cout << operand2.to_string() << " == " << operand2.to_string() << "\n";
          break;
        case NOT_EQUAL:
          cout << operand2.to_string() << " != " << operand2.to_string() << "\n";
          break;
        case GREATER:
          cout << operand2.to_string() << " > " << operand2.to_string() << "\n";
          break;
        case LESS:
          cout << operand2.to_string() << " < " << operand2.to_string() << "\n";
          break;
        case GREATER_EQUAL:
          cout << operand2.to_string() << " >= " << operand2.to_string() << "\n";
          break;
        case LESS_EQUAL:
          cout << operand2.to_string() << " <= " << operand2.to_string() << "\n";
          break;
        default:
          cout << "error comparison" << "\n";
          break;
      }
    }
    break;
  }
}
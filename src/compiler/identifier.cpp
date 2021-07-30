
#include <iostream>
#include "identifier.h"
#include "antlr4-runtime.h"

using antlr4::tree::ParseTree;
using std::cout;
using std::string;

string Result::to_string()
{
  switch (kind)
  {
    case ID:
      return id.name;
    case FLOAT:
      return std::to_string(as<float>());
    case S_INT:
      return std::to_string(as<int>());
    case INT:
      return std::to_string(as<unsigned int>());
    default:
      return "void";
  }
}

Instruction::Instruction(ParseTree* c, Abstr i, Result op1) {
  ctx = c;
  instr = i;
  operand1 = op1;
  single = true;
}

Instruction::Instruction(ParseTree* c, Abstr i, Result op1, Result op2) {
  ctx = c;
  instr = i;
  operand1 = op1;
  operand2 = op2;
  single = false;
}

Instruction::Instruction(const Instruction& other) {
  ctx = other.ctx;
  instr = other.instr;
  operand1 = other.operand1;
  single = other.single;
  condition = other.condition;
  if (!single)
    operand2 = other.operand2;
}

void Instruction::debugPrint() {
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
      // dunno yet!
    }
    break;
    case ASSIGN:
    {
      cout << operand1.to_string() << " = " << operand2.to_string() << "\n";
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
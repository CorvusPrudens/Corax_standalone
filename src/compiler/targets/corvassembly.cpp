
#include "corvassembly.h"

CorvassemblyTarget::CorvassemblyTarget(Compiler* c)
{
  comp = c;
  targetName = "Corvassembly";

  registers = {
    Register("a", Register::Data::INTEGER, Register::Rank::GENERAL, 2),
    Register("b", Register::Data::INTEGER, Register::Rank::GENERAL, 2),
    Register("c", Register::Data::INTEGER, Register::Rank::GENERAL, 2),
    Register("d", Register::Data::INTEGER, Register::Rank::GENERAL, 2),
    Register("e", Register::Data::INTEGER, Register::Rank::GENERAL, 2),

    Register("f", Register::Data::INTEGER, Register::Rank::RESERVED, 2),
    Register("g", Register::Data::INTEGER, Register::Rank::RESERVED, 2),
    Register("h", Register::Data::INTEGER, Register::Rank::RESERVED, 2),
  };
}

void CorvassemblyTarget::TranslateAdd(Instruction& inst)
{
  Register& ass = GetAss(*inst.assignment);
  Register& op1 = LoadResult(inst.operand1);
  Register& op2 = LoadResult(inst.operand2);

  // TODO -- this won't really happen like this, will it??
  if (&ass == &op1)
    AddLine("add " + op1.name + ", " + op2.name);
  else
    AddLine("add " + op1.name + ", " + op2.name + ", " + ass.name);
}
void CorvassemblyTarget::TranslateSub(Instruction& inst)
{
  unsupported(inst);
}
void CorvassemblyTarget::TranslateMult(Instruction& inst)
{
  unsupported(inst);
}
void CorvassemblyTarget::TranslateDiv(Instruction& inst)
{
  unsupported(inst);
}
void CorvassemblyTarget::TranslateMod(Instruction& inst)
{
  unsupported(inst);
}

void CorvassemblyTarget::TranslateShiftl(Instruction& inst)
{
  unsupported(inst);
}
void CorvassemblyTarget::TranslateShiftr(Instruction& inst)
{
  unsupported(inst);
}
void CorvassemblyTarget::TranslateBitAnd(Instruction& inst)
{
  unsupported(inst);
}
void CorvassemblyTarget::TranslateBitXor(Instruction& inst)
{
  unsupported(inst);
}
void CorvassemblyTarget::TranslateBitOr(Instruction& inst)
{
  unsupported(inst);
}

void CorvassemblyTarget::TranslateAnd(Instruction& inst)
{
  unsupported(inst);
}
void CorvassemblyTarget::TranslateOr(Instruction& inst)
{
  unsupported(inst);
}
void CorvassemblyTarget::TranslateCmp(Instruction& inst)
{
  unsupported(inst);
}
void CorvassemblyTarget::TranslateDeref(Instruction& inst)
{
  unsupported(inst);
}
void CorvassemblyTarget::TranslateNot(Instruction& inst)
{
  unsupported(inst);
}
void CorvassemblyTarget::TranslateNegate(Instruction& inst)
{
  unsupported(inst);
}
void CorvassemblyTarget::TranslateNegative(Instruction& inst)
{
  unsupported(inst);
}
void CorvassemblyTarget::TranslateConvert(Instruction& inst)
{
  unsupported(inst);
}
void CorvassemblyTarget::TranslateAssign(Instruction& inst)
{
  unsupported(inst);
}
// will only work if it's been loaded _and_ has a non-const loaded value
void CorvassemblyTarget::TranslateStore(Register& reg)
{
  string line = "";
  if (reg.loaded->isConst())
  {
    throw 1;
  }
  else
  {
    line += "str " + reg.name + ", " + reg.loaded->to_string();
    reg.status = Register::Status::FREE;
  }
  AddLine(line);
}
void CorvassemblyTarget::TranslateLoad(Register& reg, Result& res)
{
  string line = "ldr " + reg.name + ", " + res.to_string();
  if (!res.isConst()) reg.status = Register::Status::USED;
  AddLine(line);
}
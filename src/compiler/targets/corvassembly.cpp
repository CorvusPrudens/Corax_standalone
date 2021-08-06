
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
  if (inst.operand1.isConst())
  {
    Register& ass = GetAss(*inst.assignment);
    Register& op2 = LoadResult(inst.operand2);

    if (&ass == &op2)
      AddLine("add " + op2.name + ", " + inst.operand1.to_string());
    else
      AddLine("add " + op2.name + ", " + inst.operand1.to_string() + ", " + ass.name);

    UpdateRegister(ass);
    if (ass.loaded->id->name[0] != '$')
      ManageStorage(ass);
  }
  else if (inst.operand2.isConst())
  {
    Register& ass = GetAss(*inst.assignment);
    Register& op1 = LoadResult(inst.operand1);

    if (&ass == &op1)
      AddLine("add " + op1.name + ", " + inst.operand2.to_string());
    else
      AddLine("add " + op1.name + ", " + inst.operand2.to_string() + ", " + ass.name);

    UpdateRegister(ass);
    if (ass.loaded->id->name[0] != '$')
      ManageStorage(ass);
  }
  else // they can't both be const! (cause that would just be made an assignment)
  {
    Register& ass = GetAss(*inst.assignment);
    Register& op1 = LoadResult(inst.operand1);
    Register& op2 = LoadResult(inst.operand2);

    if (&ass == &op1)
      AddLine("add " + op1.name + ", " + op2.name);
    else if (&ass == &op2)
      AddLine("add " + op2.name + ", " + op1.name);
    else
      AddLine("add " + op1.name + ", " + op2.name + ", " + ass.name);

    UpdateRegister(ass);
    if (ass.loaded->id->name[0] != '$')
      ManageStorage(ass);
  }
  // Register& ass = GetAss(*inst.assignment);
  // Register& op1 = LoadResult(inst.operand1);
  // Register& op2 = LoadResult(inst.operand2);

  // if (&ass == &op1)
  //   AddLine("add " + op1.name + ", " + op2.name);
  // else
  //   AddLine("add " + op1.name + ", " + op2.name + ", " + ass.name);
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
// This could probably be cleverly optimized!
void CorvassemblyTarget::TranslateAssign(Instruction& inst)
{
  // if (inst.operand1.isConst())
  // {

  // }
  // else
  // {

  // }
    // Register& ass = GetAss(*inst.assignment);
    Register& op2 = LoadResult(inst.operand1);

    try {
      Register& ass = CheckLoaded(*inst.assignment);
      AddLine("add " + op2.name + ", 0, " + ass.name);
      ManageStorage(ass);
    } catch (int e) {
      // for now, since a register can only hold one pointer,
      // the loaded value will change to the assignee
      Result& temp = GenerateResult(*inst.assignment);
      // this would be ideal, but the register needs a pointer to a result!!
      op2.load(temp);
      UpdateRegister(op2);
      ManageStorage(op2);
      // StoreRegister(op2, *inst.assignment);
    }
    

    

  // if (inst.operand1.isConst())
  // {
  //   // Register& ass = GetAss(*inst.assignment);
  //   Register& op2 = LoadResult(inst.operand1);

  //   // for now, since a register can only hold one pointer,
  //   // the loaded value with change to the assignee
  //   StoreRegister(op2, *inst.assignment);

  //   // if (&ass == &op2)
  //   //   AddLine("add " + op2.name + ", " + inst.operand1.to_string());
  //   // else
  //   //   AddLine("add " + op2.name + ", " + inst.operand1.to_string() + ", " + ass.name);
  // }
  // else // they can't both be const! (cause that would just be made an assignment)
  // {
  //   Register& ass = GetAss(*inst.assignment);
  //   Register& op1 = LoadResult(inst.operand1);

  //   if (&ass == &op1)
  //     AddLine("add " + op1.name + ", " + op2.name);
  //   else if (&ass == &op2)
  //     AddLine("add " + op2.name + ", " + op1.name);
  //   else
  //     AddLine("add " + op1.name + ", " + op2.name + ", " + ass.name);
  // }
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
    // reg.status = Register::Status::FREE;
  }
  AddLine(line);
}
void CorvassemblyTarget::TranslateStore(Register& reg, Identifier& id)
{
  string line = "";
  line += "str " + reg.name + ", " + id.name;
  // reg.status = Register::Status::FREE; // not sure this should ever be used
  AddLine(line);
}
void CorvassemblyTarget::TranslateLoad(Register& reg, Result& res)
{
  string line = "ldr " + reg.name + ", " + res.to_string();
  // if (!res.isConst()) reg.status = Register::Status::USED;
  AddLine(line);
}
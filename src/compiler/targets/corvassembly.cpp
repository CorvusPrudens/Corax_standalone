
#include "corvassembly.h"

CorvassemblyTarget::CorvassemblyTarget(Compiler* c)
{
  comp = c;
  targetName = "Corvassembly";
  conditionalLabels = 0;
  operationStep = 0;

  registers = {
    Register("a", Register::Data::INTEGER, Register::Rank::GENERAL, 2),
    Register("b", Register::Data::INTEGER, Register::Rank::GENERAL, 2),
    Register("c", Register::Data::INTEGER, Register::Rank::GENERAL, 2),
    Register("d", Register::Data::INTEGER, Register::Rank::GENERAL, 2),
    Register("e", Register::Data::INTEGER, Register::Rank::GENERAL, 2),
    Register("f", Register::Data::INTEGER, Register::Rank::GENERAL, 2),

    Register("sp", Register::Data::INTEGER, Register::Rank::STACK_POINTER, 2),
    Register("bp", Register::Data::INTEGER, Register::Rank::BASE_POINTER, 2),
  };

  returnVal = &registers[0];
}

void CorvassemblyTarget::StandardInstruction(Instruction& inst, string mnemonic)
{
  if (inst.operand1.isConst())
  {
    Register& op2 = PrepareResult(inst.operand2);
    Register& ass = PrepareAssign(*inst.assignment);

    if (&ass == &op2)
      AddLine(mnemonic + " " + op2.name + ", " + inst.operand1.to_string());
    else
      AddLine(mnemonic + " " + op2.name + ", " + inst.operand1.to_string() + ", " + ass.name);

    UpdateRegister(ass);
    ManageStorage(ass);
  }
  else if (inst.operand2.isConst())
  {
    Register& op1 = PrepareResult(inst.operand1);
    Register& ass = PrepareAssign(*inst.assignment);

    if (&ass == &op1)
      AddLine(mnemonic + " " + op1.name + ", " + inst.operand2.to_string());
    else
      AddLine(mnemonic + " " + op1.name + ", " + inst.operand2.to_string() + ", " + ass.name);

    UpdateRegister(ass);
    ManageStorage(ass);
  }
  else // they can't both be const! (cause that would just be made an assignment)
  {
    // TODO -- in reality, these should all have a first pass search, and if
    // it's not already loaded somewhere, then after all the other operands
    // have had their search, we'll have a second round of searching/loading!!!
    Register& op1 = PrepareResult(inst.operand1);
    Register& op2 = PrepareResult(inst.operand2);
    Register& ass = PrepareAssign(*inst.assignment);

    if (&ass == &op1)
      AddLine(mnemonic + " " + op1.name + ", " + op2.name);
    else if (&ass == &op2)
      AddLine(mnemonic + " " + op2.name + ", " + op1.name);
    else
      AddLine(mnemonic + " " + op1.name + ", " + op2.name + ", " + ass.name);

    UpdateRegister(ass);
    ManageStorage(ass);
  }
}

void CorvassemblyTarget::TranslateAdd(Instruction& inst)
{
  StandardInstruction(inst, "add");
}
void CorvassemblyTarget::TranslateSub(Instruction& inst)
{
  StandardInstruction(inst, "sub");
}
void CorvassemblyTarget::TranslateMult(Instruction& inst)
{
  StandardInstruction(inst, "mult");
}
void CorvassemblyTarget::TranslateDiv(Instruction& inst)
{
  StandardInstruction(inst, "div");
}
void CorvassemblyTarget::TranslateMod(Instruction& inst)
{
  StandardInstruction(inst, "mod");
}

void CorvassemblyTarget::TranslateShiftl(Instruction& inst)
{
  StandardInstruction(inst, "lsl");
}
void CorvassemblyTarget::TranslateShiftr(Instruction& inst)
{
  StandardInstruction(inst, "lsr");
}
void CorvassemblyTarget::TranslateBitAnd(Instruction& inst)
{
  StandardInstruction(inst, "and");
}
void CorvassemblyTarget::TranslateBitXor(Instruction& inst)
{
  StandardInstruction(inst, "xor");
}
void CorvassemblyTarget::TranslateBitOr(Instruction& inst)
{
  StandardInstruction(inst, "or");
}

void CorvassemblyTarget::TranslateAnd(Instruction& inst)
{
  unsupported(inst);
}
void CorvassemblyTarget::TranslateOr(Instruction& inst)
{
  unsupported(inst);
}

#define AFFIRMATIVE(condition) \
AddLine("joc " condition ", " + successLabel); \
AddLine("jmp " + failLabel); \
AddLine(successLabel + ":"); \
AddLine("ldr " + ass.name + ", 1"); \
AddLine("jmp " + endLabel); \
AddLine(failLabel + ":"); \
AddLine("ldr " + ass.name + ", 0"); \
AddLine(endLabel + ":");

void CorvassemblyTarget::TranslateCmp(Instruction& inst)
{
  // if (inst.operand1.isConst()) {
  //   Register& op1 = PrepareResult(inst.operand1);
  //   AddLine("cmp " + op1.name + ", " + inst.operand2.to_string());

  // } 
  if (inst.operand2.isConst()) {
    Register& op1 = PrepareResult(inst.operand1);
    AddLine("cmp " + op1.name + ", " + inst.operand2.to_string());
  } else {
    Register& op1 = PrepareResult(inst.operand1);
    Register& op2 = PrepareResult(inst.operand2);
    AddLine("cmp " + op1.name + ", " + op2.name);
  }

  Register& ass = PrepareAssign(*inst.assignment);

  Result c1;
  c1.setValue(1);
  Result& const1 = GenerateResult(c1);
  c1.setValue(0);
  Result& const0 = GenerateResult(c1);

  string successLabel = "$succ_" + std::to_string(conditionalLabels++);
  string failLabel = "$fail_" + std::to_string(conditionalLabels++);
  string endLabel = "$end_" + std::to_string(conditionalLabels++);

  switch (inst.condition) {
    case Instruction::EQUAL:
      {
        AFFIRMATIVE("equal")
      }
      break;
    case Instruction::NOT_EQUAL:
      {
        AddLine("joc equal, " + failLabel);
        AddLine("ldr " + ass.name + ", 1");  
        AddLine("jmp " + endLabel); 
        AddLine(failLabel + ":"); 
        AddLine("ldr " + ass.name + ", 0"); 
        AddLine(endLabel + ":");
      }
      break;
    case Instruction::GREATER:
      {
        AFFIRMATIVE("greater")
      }
      break;
    case Instruction::LESS:
      {
        AFFIRMATIVE("less")
      }
      break;
    case Instruction::GREATER_EQUAL:
      {
        unsupported(inst);
      }
      break;
    case Instruction::LESS_EQUAL:
      {
        unsupported(inst);
      }
      break;
    default:
      throw 1;
  }
  UpdateRegister(ass);
}
void CorvassemblyTarget::TranslateDeref(Instruction& inst)
{
  unsupported(inst);
}
void CorvassemblyTarget::TranslateNot(Instruction& inst)
{
  Register& op1 = PrepareResult(inst.operand1);
  Register& ass = PrepareAssign(*inst.assignment);

  if (&ass == &op1)
    AddLine("not " + op1.name);
  else
    AddLine("not " + op1.name + ", " + ass.loaded->to_string());

  UpdateRegister(ass);
  ManageStorage(ass);
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
    Register& op2 = PrepareResult(inst.operand1);

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
}

void CorvassemblyTarget::TranslateCall(Instruction& inst)
{
  // the A register is the only one we'll save before the jump
  if (returnVal->loaded != nullptr && !returnVal->loaded->isConst())
    StoreRegister(*returnVal);
  for (int i = inst.args.size() - 1; i > -1; i--)
  {
    Register& pr = PrepareResult(inst.args[i]);
    AddLine("push " + pr.name);
  }
  AddLine("push <address + 4>");
  AddLine("jmp " + inst.function->name);

  size_t sp_offset = 0;
  for (auto& arg : inst.args)
    sp_offset += arg.getSize() / 2; // corvassembly is a strictly 16-bit-word language
  AddLine("add " + GetStackPointer().name + ", " + std::to_string(sp_offset + 1));
  // Register& reg = PrepareAssign(inst.assignment);
  try {
    Register& ass = CheckLoaded(*inst.assignment);
    AddLine("add " + returnVal->name + ", 0, " + ass.name);
    UpdateRegister(ass);
    ManageStorage(ass);
  } catch (int e) {
    Result& res = GenerateResult(*inst.assignment);
    returnVal->load(res);
    UpdateRegister(*returnVal);
    ManageStorage(*returnVal);
  }
}

#define FUNC_END \
AddLine("<restore used>"); \
AddLine("add bp, 0, " + GetStackPointer().name); \
AddLine("pop " + GetBasePointer().name); \
AddLine("jmp [" + GetStackPointer().name + "]");

// TODO -- this could be optimized better!!
void CorvassemblyTarget::TranslateReturn(Instruction& inst)
{
  if (inst.operand1.isConst() && inst.operand1.type == void_) {
    FUNC_END
  }
  else
  {
    Register& ret = PrepareResult(inst.operand1);
    if (&ret != returnVal) {
      StoreRegister(*returnVal);
      AddLine("add " + ret.name + ", 0, " + returnVal->name);
      returnVal->load(inst.operand1);
    }
    FUNC_END
  }
}

void CorvassemblyTarget::TranslateSetup(Instruction& inst)
{
  AddLine("<save used>");
}

void CorvassemblyTarget::TranslateIf(Instruction& inst)
{
  
}
void CorvassemblyTarget::TranslateLabel(Instruction& inst)
{
  AddLine(inst.label1->name + ":");
}
void CorvassemblyTarget::TranslateConditional(Instruction& inst)
{
  Register& op1 = PrepareResult(inst.operand1);
  Register& op2 = PrepareResult(inst.operand2);
  AddLine("cmp " + op1.name + ", " + op2.name);
  switch (inst.condition)
  {
    default:
    case Instruction::GREATER:
    {
      AddLine("joc greater, " + inst.label1->name);
      AddLine("jmp " + inst.label2->name);
    }
    break;
  }
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
  }
  AddLine(line);
}
void CorvassemblyTarget::TranslateStore(Register& reg, Identifier& id)
{
  string line = "";
  line += "str " + reg.name + ", " + id.name;
  AddLine(line);
}
void CorvassemblyTarget::TranslateLoad(Register& reg, Result& res)
{
  string line = "ldr " + reg.name + ", " + res.to_string();
  AddLine(line);
}
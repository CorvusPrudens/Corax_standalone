#include "target.h"

Register::Register(string n, Data d, Rank r, unsigned int b)
{
  name = n;
  data = d;
  bytes = b;
  rank = r;
  status = Status::FREE;
}

Register::Register(const Register& other)
{
  name = other.name;
  data = other.data;
  status = other.status;
  bytes = other.bytes;
  status = other.status;
}

void BaseTarget::Translate(Identifier& function)
{
  FuncTrans temptrans;
  temptrans.id.name = function.name;
  temptrans.id.dataType = function.dataType;
  temptrans.id.members = function.members;
  temptrans.language = targetName;
  translations.push_back(temptrans);

  for (int i = 0; i < function.function.instructions.size(); i++)
  {
    methods[(int) function.function.instructions[i].instr](function.function.instructions[i]);
  }
}

void BaseTarget::unsupported(Instruction& inst)
{
  string errmess = inst.name() + " operation is not yet support for \"" + targetName + "\"";
  comp->addRuleErr(inst.ctx, errmess);
}

int BaseTarget::freeRegisters(Register::Data data, Register::Rank rank)
{
  int free = 0;
  for (int i = 0; i < registers.size(); i++)
  {
    if (registers[i].data == data && registers[i].rank == rank && registers[i].isFree())
      free++;
  }
  return free;
}

Register BaseTarget::getFree(Register::Data data, Register::Rank rank)
{
  for (int i = 0; i < registers.size(); i++)
  {
    if (registers[i].data == data && registers[i].rank == rank && registers[i].isFree())
      return registers[i];
  }
  throw 1;
}
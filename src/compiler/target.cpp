#include <limits>
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

void Register::load(Identifier& id)
{
  loaded = &id;
  latest = id.latest;
  status = Status::USED;
}

void Register::flush()
{
  loaded = nullptr;
  latest = 0;
  status = Status::FREE;
}

void BaseTarget::TranslateAll()
{
  for (const auto id : comp->globalTable->ordered)
  {
    if (id->type == Identifier::IdType::FUNCTION)
      Translate(*id);
  }
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
    (this->*methods[(int) function.function.instructions[i].instr])(function.function.instructions[i]);
  }

  // store any remaining values before returning
  StoreAll();
  ResetRegister();
}

void BaseTarget::unsupported(Instruction& inst)
{
  string errmess = inst.name() + " operation is not yet support for \"" + targetName + "\"";
  comp->addRuleErr(inst.ctx, errmess);
}

void BaseTarget::unsupported(string mess)
{
  string errmess = mess + " operation is not yet support for \"" + targetName + "\"";
  comp->err->AddError(mess, -1, "");
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

Register& BaseTarget::getFree(Register::Data data, Register::Rank rank)
{
  for (int i = 0; i < registers.size(); i++)
  {
    if (registers[i].data == data && registers[i].rank == rank && registers[i].isFree())
      return registers[i];
  }
  throw 1;
}

void BaseTarget::ResetRegisters()
{
  for (auto& reg : registers)
  {
    reg.flush();
  }
}

void BaseTarget::StoreRegister(Register& reg)
{
  // Not totally sure how we should deal with this, but for now...
  if (reg.loaded->latest < reg.latest)
  {
    reg.loaded->latest = reg.latest;
    TranslateStore(reg);
  }
  reg.status = Register::Status::FREE;
}

void BaseTarget::StoreAll(bool include special)
{
  for (auto& reg : registers)
  {
    if (!reg.isFree() && (reg.rank != Register::Rank::RESERVED || special))
      StoreRegister(reg);
  }
}

Register::Data BaseTarget::FetchDataType(Result& res)
{
  for (auto st : StandardTypes)
  {
    if (!res.isConst() && res.id->dataType == *st)
      return datatypes[st];
    if (res.isConst() && res.type == *st)
      return datatypes[st];
  }
  throw 1;
}

Register& BaseTarget::GetLastUsed(Register::Data data, Register::Rank rank)
{
  auto lowest = std::numeric_limits<unsigned int>max();
  Register* ptr = nullptr;
  for (auto& reg : registers)
  {
    if (reg.data == data && reg.rank = rank)
    {
      if (reg.operationStep < lowest)
      {
        lowest = reg.operationStep;
        ptr = &reg;
      }
    }
  }
  if (ptr == nullptr) throw 1;
  return *ptr;
}

// TODO -- still need to figure out 'freed' condition (after assignment?)
Register& BaseTarget::LoadResult(Result& res)
{
  // Check if it's already loaded and not out-of-date
  for (auto& reg : registers)
  {
    if (reg.loaded != nullptr && *reg.loaded == res) 
    {
      if (res.isConst() || reg.latest >= res.id->latest)
      {
        reg.operationStep = operationStep++;
        return reg;
      }
    }
  }

  Register* reg;
  Register::Data d = FetchDataType(res);
  // Otherwise, look through free registers of the corresponding type
  try {
    reg = &getFree(d);
  } catch (int e) {
    // Otherwise, get the last used register of the corresponding type and
    // forcefully store its value
    reg = &GetLastUsed(d);
    TranslateStore(*reg);
  }

  reg->operationStep = operationStep++;
  reg->load(res);
  TranslateLoad(*reg, res);
  return *reg;
}

Register& BaseTarget::GetAss(Result& res)
{
  // Check if it's already loaded and not out-of-date
  for (auto& reg : registers)
  {
    if (reg.loaded != nullptr && *reg.loaded == res) 
    {
      reg.operationStep = operationStep++;
      return reg;
    }
  }

  Register* reg;
  Register::Data d = FetchDataType(res);
  // Otherwise, look through free registers of the corresponding type
  try {
    reg = &getFree(d);
  } catch (int e) {
    // Otherwise, get the last used register of the corresponding type and
    // forcefully store its value
    reg = &GetLastUsed(d);
    TranslateStore(*reg);
  }

  reg->operationStep = operationStep++;
  return *reg;
}

string BaseTarget::GetLines()
{
  string output = "";
  for (auto& func : translations)
  {
    output += func.id.name + " compiled to " + targetName + "\n";
    output += "arguments:\n";
    for (auto& arg : func.id.members)
    {
      output += "  " + arg.dataType.to_string() + " " + arg.name + "\n";
    }
    output += "output:\n";

    for (auto& line : func.translation)
      output += "  " + line + "\n";
    output += "\n";
  }
}
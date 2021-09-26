#include <limits>
#include "target.h"

Register::Register(string n, Data d, Register::Rank r, unsigned int b)
{
  loaded = nullptr;
  name = n;
  data = d;
  bytes = b;
  rank = r;
  status = Status::FREE;
  requires_storage = false;
  latest = 0;
  operationStep = 0;
}

Register::Register(const Register& other)
{
  loaded = other.loaded;
  name = other.name;
  data = other.data;
  bytes = other.bytes;
  rank = other.rank;
  status = other.status;
  requires_storage = other.requires_storage;
  latest = other.latest;
  operationStep = other.operationStep;
}

void Register::load(Result& id)
{
  loaded = &id;
  if (!id.isConst())
  {
    latest = id.id->latest;
    status = Status::USED;
  }
  else
  {
    // status = Status::FREE;
  }
}

void Register::flush()
{
  loaded = nullptr;
  latest = 0;
  status = Status::FREE;
  requires_storage = false;
  operationStep = 0;
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
  operationStep = 0;
  FuncTrans temptrans;
  temptrans.id.name = function.name;
  temptrans.id.dataType = function.dataType;
  temptrans.id.members = function.members;
  temptrans.language = targetName;
  translations.push_back(temptrans);

  for (auto& inst : function.function.instructions)
  {
    (this->*methods[(int) inst.instr])(inst);
  }

  // for (int i = 0; i < function.function.instructions.size(); i++)
  // {
  //   (this->*methods[(int) function.function.instructions[i].instr])(function.function.instructions[i]);
  // }

  // store any remaining values before returning
  StoreAll(function);
  SaveUsedRegisters(); // save all the registers that were used (prepending code)
  RestoreUsedRegisters(); // restore all the registers that were used (appending code)
  ResetRegisters();
}

void BaseTarget::UpdateRegister(Register& reg)
{
  if (reg.loaded != nullptr && !reg.loaded->isConst())
    reg.latest++;
  reg.operationStep = ++operationStep;
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
  if ((reg.loaded->id->latest < reg.latest || 
      (reg.loaded->id->dataType.qualifiers & Qualifier::VOLATILE)) && 
      reg.requires_storage
  )
  {
    reg.loaded->id->latest = reg.latest;
    TranslateStore(reg);
  }
  // reg.status = Register::Status::FREE;
  reg.requires_storage = false;
}

void BaseTarget::StoreRegister(Register& reg, Identifier& ass)
{
  // Not totally sure how we should deal with this, but for now...
  if (ass.latest < reg.latest || (ass.dataType.qualifiers & Qualifier::VOLATILE))
  {
    ass.latest = reg.latest;
    TranslateStore(reg, ass);
  }
  // reg.status = Register::Status::FREE;
  reg.requires_storage = false;
}

// NOTE -- this is only really for use at the end of the function
void BaseTarget::StoreAll(Identifier& function, bool include)
{
  for (auto& reg : registers)
  {
    if ((reg.rank == Register::Rank::GENERAL || include) && reg.requires_storage)
    {
      try{
        function.funcTable->GetLocalSymbol(reg.loaded->id->name);
        if (reg.loaded->id->dataType.qualifiers & Qualifier::VOLATILE) {
          // if it's volatile, store it anyway
          StoreRegister(reg);
        }
      } catch (int e) {
        // bit of a hacky way to determine if it's a global, but it works...
        try {
          function.funcTable->GetSymbol(reg.loaded->id->name);
          StoreRegister(reg);
        } catch (int e) {

        }
      }
    }
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

Register::Data BaseTarget::FetchDataType(Identifier& id)
{
  for (auto st : StandardTypes)
  {
    if (id.dataType == *st)
      return datatypes[st];
  }
  throw 1;
}

Register& BaseTarget::GetLastUsed(Register::Data data, Register::Rank rank)
{
  auto lowest = std::numeric_limits<unsigned int>::max();
  auto lowestFree = std::numeric_limits<unsigned int>::max();
  Register* lastToStore = nullptr;
  Register* lastFree = nullptr;
  for (auto& reg : registers)
  {
    if (reg.data == data && reg.rank == rank)
    {
      if (!reg.isFree() && reg.operationStep < lowest)
      {
        lowest = reg.operationStep;
        lastToStore = &reg;
      }
      else if (reg.isFree() && reg.operationStep < lowestFree)
      {
        lowestFree = reg.operationStep;
        lastFree = &reg;
      }
    }
  }
  if (lastFree == nullptr) {
    if (lastToStore == nullptr) throw 1;
    StoreRegister(*lastToStore);
    // TranslateStore(*lastToStore);
    return *lastToStore;
  }
  return *lastFree;
}

Register& BaseTarget::PrepareResult(Result& res)
{
  // Check if it's already loaded and not out-of-date
  for (auto& reg : registers)
  {
    if (reg.loaded != nullptr && *reg.loaded == res) 
    {
      if (res.isConst() || reg.latest >= res.id->latest)
      {
        reg.status = Register::Status::USED;
        reg.operationStep = operationStep++;
        return reg;
      }
    }
  }

  Register* reg;
  Register::Data d = FetchDataType(res);

  reg = &GetLastUsed(d);

  reg->operationStep = operationStep++;
  reg->load(res);
  reg->status = Register::Status::USED;
  TranslateLoad(*reg, res);
  return *reg;
}

// TODO -- this should look for the register with the _latest_ copy of the
// intended assignee
Register& BaseTarget::PrepareAssign(Identifier& res)
{
  // Check if it's already loaded and not out-of-date
  for (auto& reg : registers)
  {
    if (reg.loaded != nullptr && !reg.loaded->isConst() && *reg.loaded->id == res) 
    {
      reg.status = Register::Status::USED;
      reg.operationStep = operationStep++;
      return reg;
    }
  }

  Register* reg;
  Register::Data d = FetchDataType(res);
  
  reg = &GetLastUsed(d);

  Result& temp = GenerateResult(res);
  reg->load(temp);
  reg->status = Register::Status::USED;
  reg->operationStep = operationStep++;
  return *reg;
}

Register& BaseTarget::CheckLoaded(Identifier& res)
{
  // Check if it's already loaded and not out-of-date
  for (auto& reg : registers)
  {
    if (reg.loaded != nullptr && !reg.loaded->isConst() && *reg.loaded->id == res) 
    {
      reg.operationStep = operationStep++;
      return reg;
    }
  }
  throw 1;
}

Result& BaseTarget::GenerateResult(Identifier& id)
{
  Result res;
  res.setValue(id);
  temp_results.push_back(res);
  return temp_results.back();
}

Result& BaseTarget::GenerateResult(Result& res)
{
  temp_results.push_back(res);
  return temp_results.back();
}

// Frees registers after statements
void BaseTarget::TranslateStat(Instruction& inst)
{
  for (auto& reg : registers)
  {
    if (!reg.isFree() && (!reg.requires_storage || reg.loaded->id->name[0] == '$'))
    {
      reg.status = Register::Status::FREE;
    }
  }
}

void BaseTarget::ManageStorage(Register& reg)
{
  if (reg.loaded != nullptr && !reg.loaded->isConst())
  {
    if (reg.loaded->id->dataType.qualifiers & Qualifier::VOLATILE)
    {
      TranslateStore(reg);
      reg.requires_storage = false;
    }
    else
    {
      reg.requires_storage = true;
    }
  }
}

string BaseTarget::to_string()
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
    {
      output += " " + line.Print() + "\n";
    }
      
    output += "\n";
  }
  return output;
}

Register& BaseTarget::GetStackPointer()
{
  for (auto& reg : registers)
  {
    if (reg.rank == Register::Rank::STACK_POINTER)
      return reg;
  }
  throw 1;
}

Register& BaseTarget::GetBasePointer()
{
  for (auto& reg : registers)
  {
    if (reg.rank == Register::Rank::BASE_POINTER)
      return reg;
  }
  throw 1;
}

Register& BaseTarget::GetProgramCounter()
{
  for (auto& reg : registers)
  {
    if (reg.rank == Register::Rank::PROGRAM_COUNTER)
      return reg;
  }
  throw 1;
}
#ifndef TARGET_H
#define TARGET_H

#include <vector>
#include <unordered_map>
#include <string>
#include "identifier.h"
#include "type.h"
#include "compiler.h"

using std::vector;
using std::string;
using std::unordered_map;

struct FuncTrans {
  Identifier id; // NOTE -- this should only contain a name, return type, and members
  string language;
  vector<string> translation;
};

struct Register {
  enum Data { INTEGER = 0, FLOATING };
  enum Rank { GENERAL = 0, RESERVED };
  enum Status { FREE = 0, USED };

  Register(string n, Data d, Rank r, unsigned int b);
  Register(const Register& other);


  bool isFree() { return status == Status::FREE; }
  void load(Result& id);
  void flush();

  string name;
  Data data;
  Rank rank;
  Status status;
  unsigned int bytes;

  Result* loaded;
  unsigned int latest;
  unsigned int operationStep;
};

class BaseTarget {

  public:
    BaseTarget() {}
    ~BaseTarget() {}

    virtual void TranslateAll();

    /** Basic translation implementation. May not be
     *  sufficient for more advanced translations.
     */
    virtual void Translate(Identifier& function);

    virtual void TranslateAdd(Instruction& inst) { unsupported(inst); }
    virtual void TranslateSub(Instruction& inst) { unsupported(inst); }
    virtual void TranslateMult(Instruction& inst) { unsupported(inst); }
    virtual void TranslateDiv(Instruction& inst) { unsupported(inst); }
    virtual void TranslateMod(Instruction& inst) { unsupported(inst); }

    virtual void TranslateShiftl(Instruction& inst) { unsupported(inst); }
    virtual void TranslateShiftr(Instruction& inst) { unsupported(inst); }
    virtual void TranslateBitAnd(Instruction& inst) { unsupported(inst); }
    virtual void TranslateBitXor(Instruction& inst) { unsupported(inst); }
    virtual void TranslateBitOr(Instruction& inst) { unsupported(inst); }

    virtual void TranslateAnd(Instruction& inst) { unsupported(inst); }
    virtual void TranslateOr(Instruction& inst) { unsupported(inst); }
    virtual void TranslateCmp(Instruction& inst) { unsupported(inst); }

    virtual void TranslateDeref(Instruction& inst) { unsupported(inst); }
    virtual void TranslateNot(Instruction& inst) { unsupported(inst); }
    virtual void TranslateNegate(Instruction& inst) { unsupported(inst); }
    virtual void TranslateNegative(Instruction& inst) { unsupported(inst); }
    virtual void TranslateConvert(Instruction& inst) { unsupported(inst); }
    virtual void TranslateAssign(Instruction& inst) { unsupported(inst); }

    virtual void TranslateStore(Register& reg) { unsupported("register store"); }
    virtual void TranslateLoad(Register& reg, Result& res) { unsupported("register load"); }

    typedef void (BaseTarget::*TranslateMethod)(Instruction&);

    // Corresponds to Instruction's Abstr enum
    TranslateMethod methods[19] = {
      &BaseTarget::TranslateDeref,
      &BaseTarget::TranslateNot,
      &BaseTarget::TranslateNegate,
      &BaseTarget::TranslateNegative,
      &BaseTarget::TranslateConvert,
      &BaseTarget::TranslateAssign,
      &BaseTarget::TranslateAdd,
      &BaseTarget::TranslateSub,
      &BaseTarget::TranslateMult,
      &BaseTarget::TranslateDiv,
      &BaseTarget::TranslateMod,
      &BaseTarget::TranslateShiftl,
      &BaseTarget::TranslateShiftr,
      &BaseTarget::TranslateBitAnd,
      &BaseTarget::TranslateBitXor,
      &BaseTarget::TranslateBitOr,
      &BaseTarget::TranslateAnd,
      &BaseTarget::TranslateOr,
      &BaseTarget::TranslateCmp,
    };

    virtual int freeRegisters(
      Register::Data data = Register::Data::INTEGER,
      Register::Rank rank = Register::Rank::GENERAL
    );
    virtual Register& getFree(
      Register::Data data = Register::Data::INTEGER,
      Register::Rank rank = Register::Rank::GENERAL
    );
    virtual Register& GetLastUsed(
      Register::Data data = Register::Data::INTEGER,
      Register::Rank rank = Register::Rank::GENERAL
    );

    // references from the vector are safe because it
    // will never be modified after initialization
    virtual void StoreRegister(Register& reg);

    /** Stores all currently used registers
     * 
     */
    virtual void StoreAll(bool includeSpecial = false);

    /** Intelligently manages registers, either selecting a register
     * with the value already in it, loading the value into a 
     * free register, or storing another value in memory and
     * loading in that register.
     * \returns Reference to register containing newly loaded value
     */
    virtual Register& LoadResult(Result& res);
    virtual Register::Data FetchDataType(Result& res);
    virtual Register::Data FetchDataType(Identifier& id);

    /** Similar to LoadResult without loading any values --
     *  i.e. for setting up assignments
     */
    virtual Register& GetAss(Identifier& id);

    virtual void ResetRegisters();

    virtual void unsupported(Instruction& inst);
    virtual void unsupported(string mess);

    virtual void AddLine(string line) { translations.back().translation.push_back(line); }
    virtual string to_string();

    Compiler* comp;
    string targetName;
    vector<Register> registers;
    vector<FuncTrans> translations;
    unsigned int operationStep;

    // maybe a bit silly?
    unordered_map<TypeDescriptor*, Register::Data> datatypes;

};

#endif
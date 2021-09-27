#ifndef TARGET_H
#define TARGET_H

#include <vector>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include "identifier.h"
#include "type.h"
#include "compiler.h"

using std::vector;
using std::string;
using std::unordered_map;
using std::unordered_set;

struct Line; // forward decl
struct Register;

struct FuncTrans {
  Identifier id; // NOTE -- this should only contain a name, return type, and members
  string language;
  unordered_set<Register*> used_registers;

  void AddLine(Line line, vector<Register*> registers);

  vector<Line> translation;
};

struct Register {
  enum Data { INTEGER = 0, FLOATING };
  enum Rank { GENERAL = 0, STACK_POINTER, BASE_POINTER, PROGRAM_COUNTER, RESERVED };
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
  bool requires_storage;

  Result* loaded;
  unsigned int latest;
  unsigned int operationStep;
};

struct LineArg {
  enum TYPE { REGISTER = 0, RESULT, STRING };
  
  LineArg() {}
  LineArg(Register& r);
  LineArg(Result& r);
  LineArg(string s);
  // LineArg(LineArg& other);
  ~LineArg() {}

  string to_string();
  TYPE GetType() { return t; }

  Register* reg;
  Result* result;
  string str;
  TYPE t;
};

struct Line {
  Line() {}
  Line(string mnem, vector<LineArg> a);
  Line(string mnem, vector<LineArg> a, string cond);

  string to_string();

  string mnemonic;
  string condition;
  vector<LineArg> args;
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

    virtual void TranslateCall(Instruction& inst) { unsupported(inst); }
    virtual void TranslateSetup(Instruction& inst) { unsupported(inst); }
    virtual void TranslateReturn(Instruction& inst) { unsupported(inst); }
    virtual void TranslateStat(Instruction& inst);

    virtual void TranslateIf(Instruction& inst) { unsupported(inst); }
    virtual void TranslateLabel(Instruction& inst) { unsupported(inst); }
    virtual void TranslateConditional(Instruction& inst) { unsupported(inst); }

    virtual void TranslateStore(Register& reg) { unsupported("register store"); }
    virtual void TranslateStore(Register& reg, Identifier& id) { unsupported("register store"); }
    virtual void TranslateLoad(Register& reg, Result& res) { unsupported("register load"); }

    typedef void (BaseTarget::*TranslateMethod)(Instruction&);

    // Corresponds to Instruction's Abstr enum
    TranslateMethod methods[26] = {
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
      &BaseTarget::TranslateCall,
      &BaseTarget::TranslateSetup,
      &BaseTarget::TranslateReturn,
      &BaseTarget::TranslateIf,
      &BaseTarget::TranslateLabel,
      &BaseTarget::TranslateConditional,
      &BaseTarget::TranslateStat,
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
    virtual void StoreRegister(Register& reg, Identifier& ass);

    /** Stores all currently used registers
     * 
     */
    virtual void StoreAll(Identifier& function, bool includeSpecial = false);

    /** Intelligently manages registers, either selecting a register
     * with the value already in it, loading the value into a 
     * free register, or storing another value in memory and
     * loading in that register.
     * \returns Reference to register containing newly loaded value
     */
    virtual Register& PrepareResult(Result& res);
    virtual Register::Data FetchDataType(Result& res);
    virtual Register::Data FetchDataType(Identifier& id);

    /** Returns the first register with the rank of STACK_POINTER
     */
    virtual Register& GetStackPointer();

    /** Returns the first register with the rank of BASE_POINTER
     */
    virtual Register& GetBasePointer();

    /** Returns the first register with the rank of PROGRAM_COUNTER
     */
    virtual Register& GetProgramCounter();

    /** Similar to PrepareResult without loading any values --
     *  i.e. for setting up assignments
     */
    virtual Register& PrepareAssign(Identifier& id);
    virtual Register& CheckLoaded(Identifier& id);
    virtual Result& GenerateResult(Identifier& id);
    virtual Result& GenerateResult(Result& res);
    virtual void ManageStorage(Register& reg);

    virtual void UpdateRegister(Register& reg);
    virtual void ResetRegisters();

    virtual void unsupported(Instruction& inst);
    virtual void unsupported(string mess);

    virtual void SaveUsedRegisters() = 0;
    virtual void RestoreUsedRegisters() = 0;

    virtual void AddLine(string mnemonic, vector<LineArg> args);
    virtual void AddLine(string mnemonic, vector<LineArg> args, string condition);
    virtual string to_string();

    Compiler* comp;
    string targetName;
    vector<Register> registers;
    vector<FuncTrans> translations;
    Register* returnVal;
    unsigned int operationStep;
    unsigned int conditionalLabels;
    std::list<Result> temp_results;

    // maybe a bit silly?
    unordered_map<TypeDescriptor*, Register::Data> datatypes;

};

#endif
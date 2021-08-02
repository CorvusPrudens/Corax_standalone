#ifndef TARGET_H
#define TARGET_H

#include <vector>
#include <string>
#include "identifier.h"
#include "compiler.h"

using std::vector;
using std::string;

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

  string name;
  Data data;
  Rank rank;
  Status status;
  unsigned int bytes;
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

    typedef void (BaseTarget::*TranslateMethod)(Instruction& inst);

    // Corresponds to Instruction's Abstr enum
    TranslateMethod methods[19] = {
      TranslateDeref,
      TranslateNot,
      TranslateNegate,
      TranslateNegative,
      TranslateConvert,
      TranslateAssign,
      TranslateAdd,
      TranslateMult,
      TranslateDiv,
      TranslateMod,
      TranslateShiftl,
      TranslateShiftr,
      TranslateBitAnd,
      TranslateBitXor,
      TranslateBitOr,
      TranslateAnd,
      TranslateOr,
      TranslateCmp,
    };

    virtual int freeRegisters(
      Register::Data data = Register::Data::INTEGER,
      Register::Rank rank = Register::Rank::GENERAL
    );
    virtual Register getFree(
      Register::Data data = Register::Data::INTEGER,
      Register::Rank rank = Register::Rank::GENERAL
    );

    virtual void unsupported(Instruction& inst);

    Compiler* comp;
    string targetName;
    vector<Register> registers;
    vector<FuncTrans> translations;

};

#endif
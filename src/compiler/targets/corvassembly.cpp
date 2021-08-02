
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

    Register("e", Register::Data::INTEGER, Register::Rank::RESERVED, 2),
    Register("e", Register::Data::INTEGER, Register::Rank::RESERVED, 2),
    Register("e", Register::Data::INTEGER, Register::Rank::RESERVED, 2),
  };
}

#ifndef CORVASSEMBLY_H
#define CORVASSEMBLY_H

#include "target.h"
#include "compiler.h"


class CorvassemblyTarget : public BaseTarget {

  public:
    CorvassemblyTarget(Compiler* c);
    ~CorvassemblyTarget() {}

};

#endif
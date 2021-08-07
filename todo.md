# July 11, 2021

- The precompiler should remove whitespace(?) and precede every line with descriptors about its origin:

~~~ C
file "./src/main.cpp" line 72 `a += 2;
~~~

- ~~Make individual CMakeLists for compiler / precompiler and any significant subdirectories(cause compiling all that every time takes fricken forever)~~

# July 22, 2021

- ~~The compiler needs an identifier 'stack' of some sort so multiple definitions can be handled at once (for example, function arguments or struct members)~~
- The symbol table adding needs to be totally redone. It doesn't work with multiple identifiers in a single line:
~~~ c
int a, b, c;
~~~
- ~~To accomplish this, the declarator rule is where we'll add a dummy identifier to store the current type, and for each init_decl, we'll add a symbol.~~ Actually no, we just needed the temp type back.
- For some reason, the redefinition error is going nuts in the parameter list. We'll have to investigate there.

!!!
- Registers should probably be searched for existing values for all operands (op1, op2, assign) before loading those registers with their values. This maximizes reuse!
!!!

- The assembly (and architecture) needs some modification:
  - relative loads and stores should work with any register:
~~~arm
ldr a, [sp]
ldr b, [sp, 4]
ldr c, [b, -4]
~~~
  - the g and h registers should be renamed to sp and bp (stack pointer and base pointer)
  - we should be able to store data in program memory (stretch goal)
  - we need a push / pop that uses the sp register!

- With function calls, the convention will be for the callee to restore all registers is uses (i.e. a-f)
- BUG -- inline comments after code cause the error reporting to move down by a line
- Need to add implicit return (if not given)
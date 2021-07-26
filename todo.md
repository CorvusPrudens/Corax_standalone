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
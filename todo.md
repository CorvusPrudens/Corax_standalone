# July 11, 2021

- The precompiler should remove whitespace(?) and precede every line with descriptors about its origin:

~~~ C
file "./src/main.cpp" line 72 `a += 2;
~~~

- Make individual CMakeLists for compiler / precompiler and any significant subdirectories (cause compiling all that every time takes fricken forever)
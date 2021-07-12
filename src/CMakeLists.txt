cmake_minimum_required(VERSION 3.10)

# set the project name
project(Compiler VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)

set(LIB_OUTPUT_DIR "${CMAKE_HOME_DIRECTORY}/build/dist")

add_subdirectory(antlr4/runtime/Cpp)
add_subdirectory(cli11)

include_directories(
    ${CMAKE_SOURCE_DIR}/grammars/build/Post
    ${CMAKE_SOURCE_DIR}/grammars/build/Pre
    ${CMAKE_SOURCE_DIR}/compiler
    ${CMAKE_SOURCE_DIR}/precompiler
    ${CMAKE_SOURCE_DIR}/antlr4/runtime/Cpp/runtime/src
    ${CMAKE_SOURCE_DIR}/cli11/include/CLI/
    )

set (SOURCE_FILES 
    precompiler/precompiler.cpp
    compiler/compiler.cpp
    compiler/asm.cpp
    compiler/for.cpp
    compiler/function.cpp
    compiler/label.cpp
    compiler/scope.cpp
    compiler/variable.cpp
    compiler/while.cpp

    grammars/build/Post/PostParser.cpp
    grammars/build/Post/PostBaseListener.cpp
    grammars/build/Post/PostListener.cpp
    grammars/build/Post/PostLexer.cpp

    grammars/build/Pre/PreBaseListener.cpp
    grammars/build/Pre/PreLexer.cpp
    grammars/build/Pre/PreParser.cpp
    grammars/build/Pre/PreListener.cpp
    )

set (INCLUDE_FILES
    antlr4/runtime/Cpp/runtime/src/antlr4-runtime.h
    precompiler/precompiler.h
    compiler/compiler.h
    compiler/asm.h
    compiler/for.h
    compiler/function.h
    compiler/label.h
    compiler/scope.h
    compiler/variable.h
    compiler/while.h

    grammars/build/Post/PostBaseListener.h
    grammars/build/Post/PostLexer.h
    grammars/build/Post/PostParser.h
    grammars/build/Post/PostListener.h
    grammars/build/Pre/PreBaseListener.h
    grammars/build/Pre/PreParser.h
    grammars/build/Pre/PreListener.h
    grammars/build/Pre/PreLexer.h
    )

list (APPEND LINK_LIBS
    antlr4_static
    CLI11::CLI11
    )

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-attributes")

# add the executable
# add_executable(Compiler main.cpp)
add_executable(corax Corax.cpp
    ${SOURCE_FILES}
    ${INCLUDE_FILES}
    )
target_link_libraries(corax PUBLIC ${LINK_LIBS})
# add_dependencies(Cp 
#   antlr4_static 
#   Compiler
#   )

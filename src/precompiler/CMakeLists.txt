cmake_minimum_required(VERSION 3.10)

# set the project name
project(Corax VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-attributes")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../grammars/build/Pre
    ${CMAKE_CURRENT_SOURCE_DIR}/../antlr4/runtime/Cpp/runtime/src
    )

set (SOURCE_FILES 
    ./precompiler.cpp
    ../error.cpp

    ../grammars/build/Pre/PreBaseListener.cpp
    ../grammars/build/Pre/PreBaseVisitor.cpp
    ../grammars/build/Pre/PreLexer.cpp
    ../grammars/build/Pre/PreParser.cpp
    ../grammars/build/Pre/PreListener.cpp
    ../grammars/build/Pre/PreVisitor.cpp

    ../grammars/build/Pre/PreExprBaseListener.cpp
    ../grammars/build/Pre/PreExprBaseVisitor.cpp
    ../grammars/build/Pre/PreExprLexer.cpp
    ../grammars/build/Pre/PreExprParser.cpp
    ../grammars/build/Pre/PreExprListener.cpp
    ../grammars/build/Pre/PreExprVisitor.cpp
    )

set (INCLUDE_FILES
    ../antlr4/runtime/Cpp/runtime/src/antlr4-runtime.h

    ../colors.h
    ../error.h
    ../utils.h

    ./precompiler.h
    ./sar.h

    ../grammars/build/Pre/PreBaseListener.h
    ../grammars/build/Pre/PreBaseVisitor.h
    ../grammars/build/Pre/PreParser.h
    ../grammars/build/Pre/PreListener.h
    ../grammars/build/Pre/PreVisitor.h
    ../grammars/build/Pre/PreLexer.h

    ../grammars/build/Pre/PreExprBaseListener.h
    ../grammars/build/Pre/PreExprBaseVisitor.h
    ../grammars/build/Pre/PreExprLexer.h
    ../grammars/build/Pre/PreExprParser.h
    ../grammars/build/Pre/PreExprListener.h
    ../grammars/build/Pre/PreExprVisitor.h
    )

list (APPEND LINK_LIBS
    antlr4_static
    )

# add the executable
# add_executable(Compiler main.cpp)
add_library(precompiler
    ${SOURCE_FILES}
    ${INCLUDE_FILES}
    )
target_link_libraries(precompiler PUBLIC ${LINK_LIBS})
# add_dependencies(Cp 
#   antlr4_static 
#   Compiler
#   )

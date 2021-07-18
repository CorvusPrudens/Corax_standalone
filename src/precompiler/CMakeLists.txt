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

    ../grammars/build/Pre/PreParserBaseListener.cpp
    ../grammars/build/Pre/PreParserBaseVisitor.cpp
    ../grammars/build/Pre/PreLexer.cpp
    ../grammars/build/Pre/PreParser.cpp
    ../grammars/build/Pre/PreParserListener.cpp
    ../grammars/build/Pre/PreParserVisitor.cpp
    )

set (INCLUDE_FILES
    ../antlr4/runtime/Cpp/runtime/src/antlr4-runtime.h

    ../colors.h
    ../error.h
    ../utils.h

    ./precompiler.h
    ./sar.h

    ../grammars/build/Pre/PreParserBaseListener.h
    ../grammars/build/Pre/PreParserBaseVisitor.h
    ../grammars/build/Pre/PreParser.h
    ../grammars/build/Pre/PreParserListener.h
    ../grammars/build/Pre/PreParserVisitor.h
    ../grammars/build/Pre/PreLexer.h
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

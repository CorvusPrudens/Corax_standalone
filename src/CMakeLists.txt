cmake_minimum_required(VERSION 3.10)

# set the project name
project(Compiler VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(antlr4/runtime/Cpp)

include_directories(
    ${CMAKE_SOURCE_DIR}/grammars/build/Cp_post
    ${CMAKE_SOURCE_DIR}/grammars/build/Cp_pre
    ${CMAKE_SOURCE_DIR}/compiler
    ${CMAKE_SOURCE_DIR}/precompiler
    ${CMAKE_SOURCE_DIR}/antlr4/runtime/Cpp/runtime/src
    )

set (SOURCE_FILES 
    main.cpp
    precompiler/precompiler.cpp
    compiler/compiler.cpp

    grammars/build/Cp_post/Cp_postParser.cpp
    grammars/build/Cp_post/Cp_postBaseListener.cpp
    grammars/build/Cp_post/Cp_postListener.cpp
    grammars/build/Cp_post/Cp_postLexer.cpp

    grammars/build/Cp_pre/Cp_preBaseListener.cpp
    grammars/build/Cp_pre/Cp_preLexer.cpp
    grammars/build/Cp_pre/Cp_preParser.cpp
    grammars/build/Cp_pre/Cp_preListener.cpp
    )

set (INCLUDE_FILES
    antlr4/runtime/Cpp/runtime/src/antlr4-runtime.h
    precompiler/precompiler.h
    compiler/compiler.h
    grammars/build/Cp_post/Cp_postBaseListener.h
    grammars/build/Cp_post/Cp_postLexer.h
    grammars/build/Cp_post/Cp_postParser.h
    grammars/build/Cp_post/Cp_postListener.h
    grammars/build/Cp_pre/Cp_preBaseListener.h
    grammars/build/Cp_pre/Cp_preParser.h
    grammars/build/Cp_pre/Cp_preListener.h
    grammars/build/Cp_pre/Cp_preLexer.h
)

list (APPEND LINK_LIBS
    antlr4
    )

add_library ( Compiler STATIC 
    ${SOURCE_FILES}
    ${INCLUDE_FILES} )

# add the executable
# add_executable(Compiler main.cpp)
target_link_libraries(Compiler PUBLIC ${LINK_LIBS})
add_executable(Compiler main.cpp)
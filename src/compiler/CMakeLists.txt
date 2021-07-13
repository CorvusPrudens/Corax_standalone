cmake_minimum_required(VERSION 3.10)

# set the project name
project(Compiler VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../grammars/build/Post
    ${CMAKE_CURRENT_SOURCE_DIR}/../antlr4/runtime/Cpp/runtime/src
    )

set (SOURCE_FILES 
    ./compiler.cpp
    ./asm.cpp
    ./for.cpp
    ./function.cpp
    ./label.cpp
    ./scope.cpp
    ./variable.cpp
    ./while.cpp

    ../error.cpp

    ../grammars/build/Post/PostParser.cpp
    ../grammars/build/Post/PostBaseListener.cpp
    ../grammars/build/Post/PostListener.cpp
    ../grammars/build/Post/PostLexer.cpp
    )

set (INCLUDE_FILES
    ../antlr4/runtime/Cpp/runtime/src/antlr4-runtime.h

    ../colors.h
    ../error.h
    ../utils.h

    # pre./sar.h
    ./compiler.h
    ./asm.h
    ./for.h
    ./function.h
    ./label.h
    ./scope.h
    ./variable.h
    ./while.h

    ../grammars/build/Post/PostBaseListener.h
    ../grammars/build/Post/PostLexer.h
    ../grammars/build/Post/PostParser.h
    ../grammars/build/Post/PostListener.h
    )

list (APPEND LINK_LIBS
    antlr4_static
    )

# add the executable
# add_executable(Compiler main.cpp)
add_library(compiler
    ${SOURCE_FILES}
    ${INCLUDE_FILES}
    )
target_link_libraries(compiler PUBLIC 
                      ${LINK_LIBS})
# add_dependencies(Cp 
#   antlr4_static 
#   Compiler
#   )

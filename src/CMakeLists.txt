cmake_minimum_required(VERSION 3.10)

# set the project name
project(Corax VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

set(LIB_OUTPUT_DIR "${CMAKE_HOME_DIRECTORY}/build/dist")

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-attributes")

add_subdirectory(antlr4/runtime/Cpp)
add_subdirectory(cli11)
add_subdirectory(compiler)
add_subdirectory(precompiler)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/grammars/build/Post
    ${CMAKE_SOURCE_DIR}/grammars/build/Pre
    ${CMAKE_SOURCE_DIR}/compiler
    ${CMAKE_SOURCE_DIR}/precompiler
    ${CMAKE_SOURCE_DIR}/antlr4/runtime/Cpp/runtime/src
    ${CMAKE_SOURCE_DIR}/cli11/include/CLI/
    )

set (SOURCE_FILES 
    error.cpp
    )

set (INCLUDE_FILES
    # antlr4/runtime/Cpp/runtime/src/antlr4-runtime.h

    colors.h
    error.h
    utils.h

    precompiler/precompiler.h
    compiler/compiler.h
    )

list (APPEND LINK_LIBS
    antlr4_static
    CLI11::CLI11
    compiler
    precompiler
    )

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

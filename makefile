ANTLR_HOME = /usr/local/lib
ANTLR_JAR = ${ANTLR_HOME}/antlr-4.9.1-complete.jar

ANTLR = java -jar ${ANTLR_JAR}
GRUN = java org.antlr.v4.gui.TestRig

SRC  = ./src
ANTLR4 = antlr4
GRAMMARS = grammars
POST   = Post
PRE = Pre
JAVA_PATH = build
BUILD_PATH = ${SRC}/build

ASSEMBLER = ./assembler/assembler.py
EXAMPLE  = ./examples/test.cx

all: build_compiler

update: update_compiler build_compiler

grammar: build_cpp build_compiler

test: post_java_build post_java_test

test_pre: pre_java_build pre_java_test

.PHONY: all test clean build_cpp build_java parse_python test_java build_compiler update grammar

build_cpp: ${SRC}/${GRAMMARS}/${POST}.g4 ${SRC}/${GRAMMARS}/${PRE}.g4
	$(info Building main compiler...)
	@ cd ${SRC}/${GRAMMARS}; ${ANTLR} -Dlanguage=Cpp -package Post -o build/${POST} ${POST}.g4;
	$(info Building pre-compiler...)
	@ cd ${SRC}/${GRAMMARS}; ${ANTLR} -Dlanguage=Cpp -package Pre -o build/${PRE} ${PRE}.g4;

build_compiler: ${SRC}/${GRAMMARS}/build/${POST} ${SRC}/${GRAMMARS}/build/${PRE}
	@ if [ ! -d "${SRC}/build" ]; then $(MAKE) update_compiler; fi
	cd ${SRC}/build; make;

update_compiler:
	@ if [ ! -d "${SRC}/build" ]; then mkdir ${SRC}/build; cd ..; cmake . -B build; fi

parse_python: ${ASSEMBLER} ${EXAMPLE}
	python3 test.py

test_python:
	cd tests; py.test;
	
post_java_build: ${SRC}/${GRAMMARS}/${POST}.g4
	$(info Building main compiler parse tree...)
	@ cd ${SRC}/${GRAMMARS}; ${ANTLR} -o ${JAVA_PATH}/${POST}_java ${POST}.g4; \
	javac -d ${JAVA_PATH}/${POST}_java ${JAVA_PATH}/${POST}_java/${POST}*.java

post_java_test: ${SRC}/${EXAMPLE}
	$(info Executing main compiler parse tree...)
	@ cd ${SRC}/${GRAMMARS}/${JAVA_PATH}/${POST}_java; \
	${GRUN} ${POST} parse ../../.${EXAMPLE} -gui

pre_java_build: ${SRC}/${GRAMMARS}/${PRE}Parser.g4
	$(info Building pre-compiler parser...)
	@ cd ${SRC}/${GRAMMARS}; ${ANTLR} -o ${JAVA_PATH}/${PRE}_java ${PRE}Lexer.g4; \
	${ANTLR} -o ${JAVA_PATH}/${PRE}_java ${PRE}Parser.g4; \
	cd ${JAVA_PATH}/${PRE}_java; javac ${PRE}*.java

pre_java_test: ${SRC}/${EXAMPLE}
	$(info Executing pre-compiler parse tree...)
	@ cd ${SRC}/${GRAMMARS}/${JAVA_PATH}/${PRE}_java; \
	${GRUN} ${PRE} parse ../../.${EXAMPLE} -gui

clean:
	@ if [ -d "${SRC}/${GRAMMARS}/build" ]; then rm -r ${SRC}/${GRAMMARS}/build; fi

reset: clean
	@ if [ -d "${SRC}/build" ]; then rm -rf ${SRC}/build; fi
	@ if [ -d "${SRC}/dist" ]; then rm -rf ${SRC}/dist; fi

run: ${SRC}/build/corax
	${SRC}/build/corax ${SRC}/examples/test.cx
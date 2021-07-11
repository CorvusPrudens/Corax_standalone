ANTLR_HOME = /usr/local/lib
ANTLR_JAR = ${ANTLR_HOME}/antlr-4.9.1-complete.jar

ANTLR = java -jar ${ANTLR_JAR}
GRUN = java org.antlr.v4.gui.TestRig

SRC  = ./src
ANTLR4 = antlr4
GRAMMARS = grammars
POST   = Cp_post
PRE = Cp_pre
JAVA_PATH = build
BUILD_PATH = ${SRC}/build

ASSEMBLER = ./assembler/assembler.py
EXAMPLE  = ./examples/test.cp

all: build_cpp

test: post_java_build post_java_test

test_pre: pre_java_build pre_java_test

.PHONY: all test clean build_cpp build_java parse_python test_java test_python

build_cpp: ${SRC}/${GRAMMARS}/${POST}.g4 ${SRC}/${GRAMMARS}/${PRE}.g4
	$(info Building main compiler...)
	@ cd ${SRC}/${GRAMMARS}; ${ANTLR} -Dlanguage=Cpp -package Post -o build/${POST} ${POST}.g4;
	$(info Building pre-compiler...)
	@ cd ${SRC}/${GRAMMARS}; ${ANTLR} -Dlanguage=Cpp -package Pre -o build/${PRE} ${PRE}.g4;

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

pre_java_build: ${SRC}/${GRAMMARS}/${PRE}.g4
	$(info Building pre-compiler parse tree...)
	@ cd ${SRC}/${GRAMMARS}; ${ANTLR} -o ${JAVA_PATH}/${PRE}_java ${PRE}.g4; \
	javac -d ${JAVA_PATH}/${PRE}_java ${JAVA_PATH}/${PRE}_java/${PRE}*.java

pre_java_test: ${SRC}/${EXAMPLE}
	$(info Executing pre-compiler parse tree...)
	@ cd ${SRC}/${GRAMMARS}/${JAVA_PATH}/${PRE}_java; \
	${GRUN} ${PRE} parse ../../.${EXAMPLE} -gui

clean:
	@ if [ -d "${SRC}/${GRAMMARS}/build" ]; then rm -r ${SRC}/${GRAMMARS}/build; fi
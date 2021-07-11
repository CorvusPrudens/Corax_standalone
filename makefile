ANTLR_HOME = /usr/local/lib
ANTLR_JAR = ${ANTLR_HOME}/antlr-4.9.1-complete.jar

ANTLR = java -jar ${ANTLR_JAR}
GRUN = java org.antlr.v4.gui.TestRig

SRC       = ./src
GRAMMAR   = Cp.g4
JAVA_PATH = build
# GRAMMAR = ./src/Cp.g4
PYTHON_PATH = ./assembler/gen
# JAVA_PATH = ./src/build

ASSEMBLER = ./assembler/assembler.py
EXAMPLE  = ./examples/test.cp

all: build_python parse_python test_python

test: build_java test_java

clean: clean_java clean_python

.PHONY: all test clean build_python build_java parse_python test_java clean_java clean_python test_python

build_python: ${GRAMMAR}
	${ANTLR} -Dlanguage=Python3 -o ${PYTHON_PATH} ${GRAMMAR}

parse_python: ${ASSEMBLER} ${EXAMPLE}
	python3 test.py

test_python:
	cd tests; py.test;
	
build_java: ${SRC}/${GRAMMAR}
	cd ${SRC}; ${ANTLR} -o ${JAVA_PATH} ${GRAMMAR}; \
	javac -d ${JAVA_PATH} ${JAVA_PATH}/Cp*.java

test_java: ${SRC}/${EXAMPLE}
	cd ${SRC}/${JAVA_PATH}; \
	${GRUN} Cp parse .${EXAMPLE} -gui

clean_python:
	@ if [ -d "${PYTHON_PATH}" ]; then rm -r ${PYTHON_PATH}; fi

clean_java:
	@ if [ -d "${JAVA_PATH}" ]; then rm -r ${JAVA_PATH}; fi
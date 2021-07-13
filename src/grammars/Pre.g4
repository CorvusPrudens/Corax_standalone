grammar Pre;

parse         : anything* EOF;

anything      : directive | anything_else;

directive     : include_ | if_ | ifdef_ | ifndef_ | define_;

include_      : INCLUDE STRING NEWLINE;

if_           : IF condition? NEWLINE anything* (endif_ | else_);

ifdef_        : IFDEF NAME NEWLINE anything* (endif_ | else_);

ifndef_       : IFNDEF NAME NEWLINE anything* (endif_ | else_);

else_         : ELSE NEWLINE anything* endif_;
 
endif_        : ENDIF NEWLINE;

define_       : DEFINE NAME anything_else* NEWLINE;

condition     : OPAREN (expression || expression COMPARATOR expression); 

expression    : (NAME | NUMBER) (OPERATOR (NAME | NUMBER))*;

anything_else : (ANYTHING | NAME | NUMBER | OPERATOR | COMPARATOR | OPAREN | CPAREN | NEWLINE)+;

// Lexy

STRING        : '"' (~["\r\n] | '""' | '\\"')* '"';
COMMENT       : '//' ~[\n\r]* [\n\r] -> skip;
COMMENT_BLOCK : '/*' .*? '*/' -> skip;
TEST_BLOCK    : '$' .*? '$end' -> skip;

INCLUDE       : '#include';
IF            : '#if';
IFDEF         : '#ifdef';
IFNDEF        : '#ifndef';
ELSE          : '#else';
ENDIF         : '#endif';
DEFINE        : '#define';

OPAREN        : '(';
CPAREN        : ')';
NEWLINE       : [\n\r];

NAME          : [a-zA-Z_][a-zA-Z_0-9]*;
OPERATOR      : [+\-*/%] | '**';
COMPARATOR    : [><] '='? | '==';

fragment DEC  : [1-9][0-9_]* | '0';
fragment HEX  : '0x'[0-9A-Fa-f][0-9A-Fa-f_]*;
fragment BIN  : '0b'[0-1][0-1_]*;
fragment FLT  : ([1-9][0-9_]* | '0') '.' ([1-9][0-9_]* | '0');
NUMBER        : DEC | BIN | HEX | FLT;

WHITESPACE    : [ \t] -> skip;
ANYTHING      : .+?;
grammar Cp;

// parsey

parse         : (function | variable_init | struct_)* EOF;

scope         : OBRACE (call | variable_init | statement)* CBRACE;

variable_init : variable_def 
              | variable_dec;

variable_def : type NAME assignment? SEMI;

variable_dec : type NAME array_init? SEMI;

call         : variable OPAREN ( | VOID | NAME (COMMA NAME)* COMMA?) CPAREN SEMI;

variable     : NAME accessor?;

accessor     : arrow 
             | dot 
             | indexer;

arrow        : ARROW variable;

dot          : DOT variable;

indexer      : OBRACKET expression CBRACKET;

array_init   : OBRACKET expression? CBRACKET;

type         : NAME 
             | VOID;

assignment   : (EQUALS expression)
             | (array_init EQUALS OBRACE expression (COMMA expression)* COMMA? CBRACE);

// macros will be handled by a proper pre-processor
expression   : NUMBER 
             | variable (INCREMENT | DECREMENT)?;

// Obviously this will need expansion
statement    : variable (EQUALS | COMPOUND) expression (OPERATOR expression)* SEMI;

struct_      : STRUCT NAME EQUALS OBRACE variable+ CBRACE SEMI;

function     : function_def 
             | function_dec SEMI;

function_dec : type NAME OPAREN ( | VOID | type NAME (COMMA type NAME)* COMMA?) CPAREN;

function_def : function_dec scope;

STRING                 : '"' (~["\r\n] | '""' | '\\"')* '"';
CHAR                   : '\'' (('\\'.)|~[\\]) '\'';
COMMENT                : '//' ~[\n\r]* [\n\r] -> skip;
COMMENT_BLOCK          : '/*' .*? '*/' -> skip;
TEST_BLOCK             : '$' .*? '$end' -> skip;
PRAGMA                 : '#pragma';

NAME                   : [a-zA-Z_][a-zA-Z_0-9]*;

OBRACE                 : '{';
CBRACE                 : '}';
OBRACKET               : '[';
CBRACKET               : ']';
OPAREN                 : '(';
CPAREN                 : ')';
COMMA                  : ',';
SEMI                   : ';';

ARROW                  : '->';
DOT                    : '.';

VOID                   : 'void';
STRUCT                 : 'struct';

COMPOUND               : [+\-*/] '=';
OPERATOR               : [+\-*/%] | '**';
COMPARATOR             : [><] '='? | '==';

EQUALS                 : '=';

INCREMENT              : '++';
DECREMENT              : '--';

fragment DEC           : [1-9][0-9_]* | '0';
fragment HEX           : '0x'[0-9A-Fa-f][0-9A-Fa-f_]*;
fragment BIN           : '0b'[0-1][0-1_]*;
fragment FLT           : ([1-9][0-9_]* | '0') '.' ([1-9][0-9_]* | '0');
NUMBER                 : DEC | BIN | HEX | FLT;
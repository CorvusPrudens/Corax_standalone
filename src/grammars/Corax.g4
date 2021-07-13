grammar Corax;

// parsey

parse         : (function | variable_init | struct_)* EOF;

scope         : OBRACE (variable_init | statement | label)* CBRACE;

control       : return_ | break_ | breakall | continue_;

return_       : RETURN expression? SEMI;

break_        : BREAK SEMI;

breakall      : BREAKALL SEMI;

continue_     : CONTINUE SEMI;

label         : NAME COLON;

variable_init : variable_def 
              | variable_dec;

variable_def : type (assignment | variable list) SEMI;

variable_dec : type NAME array_init? SEMI;

call         : variable OPAREN ( | type | expression (COMMA expression)* COMMA?) CPAREN SEMI;

array_init   : OBRACKET expression? CBRACKET;

type         : NAME;

list         : (array_init EQUALS OBRACE expression (COMMA expression)* COMMA? CBRACE);

variable     : NAME accessor?;

accessor     : arrow 
             | dot 
             | indexer;

arrow        : ARROW variable;

dot          : DOT variable;

indexer      : OBRACKET expression CBRACKET;

// macros will be handled by a proper pre-processor
operand      : NUMBER 
             | variable
             | preop
             | postop
             ;

preop        : adjacent variable;

postop       : variable adjacent;

adjacent     : INCREMENT | DECREMENT;

statement    : assignment SEMI | control | if_ | while_ | for_ | call | preop SEMI | postop SEMI | empty;

empty        : SEMI;

assignment   : variable (EQUALS | COMPOUND) expression;

expression   : combination (OPERATOR combination)*;

combination  : operand
             | subgroup
             ;

subgroup     : OPAREN operand (OPERATOR operand)* CPAREN;

struct_      : STRUCT NAME EQUALS OBRACE variable+ CBRACE SEMI;

if_          : IF condition (scope | statement) else_*;

else_        : ELSE (IF condition)? (scope | statement);

condition    : OPAREN (expression | conditional) CPAREN;

conditional  : expression COMPARATOR expression;

for_         : FOR for_construct (scope | statement);

for_construct : OPAREN (statement | variable_init) (expression SEMI | conditional SEMI | empty) for_end CPAREN;

for_end       : | assignment | preop | postop;

while_       : WHILE condition (statement | scope);

function     : function_def 
             | function_dec SEMI;

function_dec : type NAME OPAREN ( | type | type NAME (COMMA type NAME)* COMMA?) CPAREN;

function_def : function_dec scope;

STRING                 : '"' (~["\r\n] | '""' | '\\"')* '"';
CHAR                   : '\'' (('\\'.)|~[\\]) '\'';
COMMENT                : '//' ~[\n\r]* [\n\r] -> skip;
COMMENT_BLOCK          : '/*' .*? '*/' -> skip;
TEST_BLOCK             : '$' .*? '$end' -> skip;
PRAGMA                 : '#pragma';

OBRACE                 : '{';
CBRACE                 : '}';
OBRACKET               : '[';
CBRACKET               : ']';
OPAREN                 : '(';
CPAREN                 : ')';
COMMA                  : ',';
SEMI                   : ';';
COLON                  : ':';

ARROW                  : '->';
DOT                    : '.';

STRUCT                 : 'struct';
IF                     : 'if';
ELSE                   : 'else';
FOR                    : 'for';
WHILE                  : 'while';
RETURN                 : 'return';
BREAK                  : 'break';
BREAKALL               : 'breakall';
CONTINUE               : 'continue';

NAME                   : [a-zA-Z_][a-zA-Z_0-9]*;

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
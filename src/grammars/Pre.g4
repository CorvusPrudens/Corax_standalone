grammar Pre;

parse         : anything* EOF;

anything      : directive      #topDirective
              | anything_else+ #topAny
              | NEWLINE        #topNewline
              ;

directive     : include_ #include
              | if_      #if
              | ifdef_   #ifdef
              | ifndef_  #ifndef
              | define_  #define
              | undef_   #undef
              | line_    #line
              | error_   #error
              | empty_   #empty
              ;

include_      : INCLUDE STRING NEWLINE  #string
              | INCLUDE LIBRARY NEWLINE #library
              ;

if_           : IF anything_expr NEWLINE anything* (endif_ | elif_ | else_);

elif_         : ELIF anything_expr NEWLINE anything* (endif_ | elif_ | else_);

ifdef_        : IFDEF NAME NEWLINE anything* (endif_ | else_);

ifndef_       : IFNDEF NAME NEWLINE anything* (endif_ | else_);

else_         : ELSE NEWLINE anything* endif_;
 
endif_        : ENDIF NEWLINE;

define_       : DEFINE NAME anything_expr? NEWLINE;

undef_        : UNDEF NAME NEWLINE;

line_         : LINE number NEWLINE;

error_        : ERROR STRING NEWLINE;

pragma_       : PRAGMA anything* NEWLINE;

empty_        : HASH NEWLINE;

number        : DEC #dec
              | BIN #bin
              | HEX #hex
              | OCT #oct
              | FLT #flt
              ;

anything_expr : anything_opt+;

anything_opt  : DEFINED NAME        #anyPass
              | DEFINED LP NAME RP  #anyPass
              | NAME                #anyName
              | LP                  #anyPass
              | RP                  #anyPass
              | PP                  #anyPass
              | number              #anyPass
              | STRING              #anyPass
              | HASH                #anyPass
              | HASH_DOUBLE         #anyPass
              ;

anything_else : (anything_expr | NEWLINE)+;       


// lexy
ESCAPED_NEW   : '\\' '\r'? '\n' -> skip;
NEWLINE       : '\r'? '\n';

STRING        : '"' (~["\r\n] | '""' | '\\"')* '"';
LIBRARY       : '<' [a-zA-Z_0-9./\-]+ '>';
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
ERROR         : '#error';
ELIF          : '#elif';
UNDEF         : '#undef';
LINE          : '#line';
PRAGMA        : '#pragma';
HASH          : '#';
HASH_DOUBLE   : '##';
DEFINED       : 'defined';
LP            : '(';
RP            : ')';

NAME          : [a-zA-Z_][a-zA-Z_0-9]*;

DEC           : [1-9][0-9_]* | '0';
HEX           : '0x'[0-9A-Fa-f][0-9A-Fa-f_]*;
BIN           : '0b'[0-1][0-1_]*;
OCT           : '0'[0-7]+;
FLT           : ([1-9][0-9_]* | '0') '.' ([1-9][0-9_]* | '0');

WHITESPACE    : [ \t] -> channel(HIDDEN);
PP            : .+?;
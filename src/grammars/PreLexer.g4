lexer grammar PreLexer;

NAME          : [a-zA-Z_][a-zA-Z_0-9]*;
ANYTHING      : (.|[ \t\r\n])+?;

INCLUDE       : '#include' -> mode(PRE);
IF            : '#if'      -> mode(PRE);
IFDEF         : '#ifdef'   -> mode(PRE);
IFNDEF        : '#ifndef'  -> mode(PRE);
ELSE          : '#else'    -> mode(PRE);
ENDIF         : '#endif'   -> mode(PRE);
DEFINE        : '#define'  -> mode(PRE);
ERROR         : '#error'   -> mode(PRE);
ELIF          : '#elif'    -> mode(PRE);
UNDEF         : '#undef'   -> mode(PRE);
LINE          : '#line'    -> mode(PRE);
PRAGMA        : '#pragma'  -> mode(PRE);

mode PRE;

DEFINED       : 'defined';

LP            : '(';
RP            : ')';

ESCAPED_NEW   : '\\' '\r'? '\n' -> skip;

NEWLINE       : '\r'? '\n' -> mode(DEFAULT_MODE);

STRING        : '"' (~["\r\n] | '""' | '\\"')* '"';
LIBRARY       : '<' .+? '>';
COMMENT       : '//' ~[\n\r]* [\n\r] -> skip;
COMMENT_BLOCK : '/*' .*? '*/' -> skip;
TEST_BLOCK    : '$' .*? '$end' -> skip;

ID            : [a-zA-Z_][a-zA-Z_0-9]*;

HASH          : '#';
EXCL          : '!';
PLUS          : '+';
MINUS         : '-';
NOT           : '~';
MULT          : '*';
DIV           : '/';
MOD           : '%';
SHIFT_LEFT    : '<<';
SHIFT_RIGHT   : '>>';
LESS          : '<';
GREATER       : '>';
LESS_EQUAL    : '<=';
GREATER_EQUAL : '>=';
EQUAL         : '==';
NOT_EQUAL     : '!=';
AND           : '&&';
OR            : '||';
BIT_AND       : '&';
BIT_XOR       : '^';
BIT_OR        : '|';

DEC           : [1-9][0-9_]* | '0';
HEX           : '0x'[0-9A-Fa-f][0-9A-Fa-f_]*;
BIN           : '0b'[0-1][0-1_]*;
OCT           : '0'[0-7]+;
FLT           : ([1-9][0-9_]* | '0') '.' ([1-9][0-9_]* | '0');

WHITESPACE    : [ \t] -> channel(HIDDEN);
PP            : .+?;
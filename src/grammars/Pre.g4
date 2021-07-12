grammar Pre;

import Corax;

parse      : (directive | pre_expression)* EOF;

scope      : OBRACE (call | variable_init | statement | directive)* CBRACE;

directive  : define
           | include
           | if_
           | ifdef
           | ifndef
           | else_
           | endif
           ;

pre_constant   : NAME | NUMBER;

pre_expression : function | variable_init | struct_;

math_pre       : pre_constant (OPERATOR pre_constant)*;

define         : HASH DEFINE NAME expression?;

include        : HASH INCLUDE STRING;

if_            : HASH IF (math_pre | math_pre COMPARATOR math_pre);

ifdef          : HASH IFDEF NAME;

ifndef         : HASH IFNDEF NAME;

else_          : HASH ELSE;

endif          : HASH ENDIF;


// lexy
HASH                   : '#';
DEFINE                 : 'define';
INCLUDE                : 'include';
IF                     : 'if';
IFDEF                  : 'ifdef';
IFNDEF                 : 'ifndef';
ELSE                   : 'else';
ENDIF                  : 'endif';

WHITESPACE             : [ \t\n\r] -> skip;
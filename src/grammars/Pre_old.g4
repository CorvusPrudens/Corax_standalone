grammar Pre;

import Corax;

parse      : (directive | pre_expression)* EOF;

scope      : OBRACE (variable_init | statement | label | directive)* CBRACE;

directive  : define
           | include
           | if_pre
           | ifdef
           | ifndef
           | else_pre
           | endif
           ;

pre_constant   : NAME | NUMBER;

pre_expression : function | variable_init | struct_;

math_pre       : pre_constant (OPERATOR pre_constant)*;

define         : HASH DEFINE NAME operand?;

include        : HASH INCLUDE STRING;

if_pre         : HASH IF (math_pre | math_pre COMPARATOR math_pre);

ifdef          : HASH IFDEF NAME;

ifndef         : HASH IFNDEF NAME;

else_pre       : HASH ELSE;

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
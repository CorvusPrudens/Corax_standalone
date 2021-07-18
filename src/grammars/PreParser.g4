parser grammar PreParser;

// Lexy
options { tokenVocab=PreLexer; }

parse         : anything* EOF;

not_directive : ANYTHING+ #anyPass
              | NAME      #anyName
              ;

anything      : directive      #topDirective
              | not_directive+ #topAny
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

if_           : IF expression NEWLINE anything* (endif_ | elif_ | else_);

elif_         : ELIF expression NEWLINE anything* (endif_ | elif_ | else_);

ifdef_        : IFDEF ID NEWLINE anything* (endif_ | else_);

ifndef_       : IFNDEF ID NEWLINE anything* (endif_ | else_);

else_         : ELSE NEWLINE anything* endif_;
 
endif_        : ENDIF NEWLINE;

define_       : DEFINE ID anything_else? NEWLINE;

undef_        : UNDEF ID NEWLINE;

line_         : LINE number NEWLINE;

error_        : ERROR STRING NEWLINE;

pragma_       : PRAGMA anything* NEWLINE;

empty_        : HASH NEWLINE;

expression    : DEFINED expression                  # defined
              | EXCL expression                     # negation 
              | PLUS expression                     # positive
              | MINUS expression                    # negative
              | NOT expression                      # not
              | expression MULT expression          # mult
              | expression DIV expression           # div
              | expression MOD expression           # mod
              | expression PLUS expression          # plus
              | expression MINUS expression         # minus
              | expression SHIFT_LEFT expression    # shift_left
              | expression SHIFT_RIGHT expression   # shift_right
              | expression LESS expression          # less
              | expression GREATER expression       # greater
              | expression LESS_EQUAL expression    # less_equal
              | expression GREATER_EQUAL expression # greater_equal
              | expression EQUAL expression         # equal
              | expression NOT_EQUAL expression     # not_equal
              | expression BIT_AND expression       # bit_and
              | expression BIT_XOR expression       # bit_xor
              | expression BIT_OR expression        # bit_or
              | expression AND expression           # and
              | expression OR expression            # or
              | ID                                  # expr_name
              | number                              # const
              | LP expression RP                    # group
              ;

number        : DEC #dec
              | BIN #bin
              | HEX #hex
              | OCT #oct
              | FLT #flt
              ;

anything_else : anything_opt+;

anything_opt  : PP         #dirAny
              | ID         #dirName
              | STRING     #dirAny
              | number     #dirAny
              | DOUBLE_HASH#dirDHash
              | HASH       #dirHash
              | EXCL       #dirAny
              | LP         #dirAny
              | RP         #dirAny
              | NEWLINE    #dirAny
              | PLUS       #dirAny
              | MINUS      #dirAny
              | NOT        #dirAny
              | MULT       #dirAny
              | DIV        #dirAny
              | MOD        #dirAny
              | SHIFT_LEFT #dirAny
              | SHIFT_RIGHT#dirAny
              | LESS       #dirAny
              | GREATER    #dirAny
              | LESS_EQUAL #dirAny
              | GREATER_EQUAL#dirAny
              | EQUAL      #dirAny
              | NOT_EQUAL  #dirAny
              | AND        #dirAny
              | OR         #dirAny
              | BIT_AND    #dirAny
              | BIT_XOR    #dirAny
              | BIT_OR     #dirAny
              ;
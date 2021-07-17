parser grammar PreParser;

// Lexy
options {tokenVocab=PreLexer; }

parse         : anything* EOF;

not_directive : ANYTHING+ #ignore
              | NAME      #top_name
              ;

anything      : directive      #top_directive
              | not_directive+ #top_anything
              ;

directive     : include_ #include
              | if_      #if
              | ifdef_   #ifdef
              | ifndef_  #ifndef
              | define_  #define
              | undef_   #undef
              | line_    #line
              | error_   #error
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

empty         : HASH NEWLINE;

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

anything_opt  : PP         #any
              | ID         #name
              | STRING     #any
              | number     #any
              | EXCL       #any
              | LP         #any
              | RP         #any
              | NEWLINE    #any
              | DEFINED    #any
              | PLUS       #any
              | MINUS      #any
              | NOT        #any
              | MULT       #any
              | DIV        #any
              | MOD        #any
              | SHIFT_LEFT #any
              | SHIFT_RIGHT #any
              | LESS       #any
              | GREATER    #any
              | LESS_EQUAL #any
              | GREATER_EQUAL #any
              | EQUAL      #any
              | NOT_EQUAL  #any
              | AND        #any
              | OR         #any
              | BIT_AND    #any
              | BIT_XOR    #any
              | BIT_OR     #any
              ;
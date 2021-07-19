grammar CoraxExpr;

// NOTE -- constant expressions will need to be identified through semantics
              // Primary
expression    : IDENTIFIER                   # identifier
              | NUMBER                       # const
              | STRING                       # string
              | '(' expression ')'           # group

              // postfix
              | expression '[' expression ']'# indexing
              | expression '(' arguments ')' # call // (go to page 70)
              | expression '.' expression    # member
              | expression '->' expression   # indirectMember
              | expression '++'              # incrementPost
              | expression '--'              # decrementPost
              | '(' typename ')' '{' initializer ','? '}' #idk // (page 69)
              
              // Unary
              | '++' expression              # incrementUnary
              | '--' expression              # decrementUnary
              | '&' expression               # address
              | '*' expression               # dereference
              | '+' expression               # positive 
              | '-' expression               # negative
              | '~' expression               # not
              | '!' expression               # negation
              | 'sizeof' expression          # sizeof
              | 'sizeof' '(' typename ')'    # sizeofType

              // Multiplicative
              | expression '*' expression    # mult
              | expression '/' expression    # div
              | expression '%' expression    # mod

              // Additive
              | expression '+' expression    # plus
              | expression '-' expression    # minus

              // Bitwise shift
              | expression '<<' expression   # shift_left
              | expression '>>' expression   # shift_right

              // Relational
              | expression '<' expression    # less
              | expression '>' expression    # greater
              | expression '<=' expression   # less_equal
              | expression '>=' expression   # greater_equal

              // Equality
              | expression '==' expression   # equal
              | expression '!=' expression   # not_equal

              // Bitwise
              | expression '&' expression    # bit_and
              | expression '^' expression    # bit_xor
              | expression '|' expression    # bit_or

              // Logical
              | expression '&&' expression   # and
              | expression '||' expression   # or

              // Ternary
              | expression '?' expression ':' expression # ternary

              // Assignment (NOTE -- lhs can only be primary, postfix, or unary)
              | expression '=' expression    # assignment
              | expression '*=' expression   # assignmentMult
              | expression '/=' expression   # assignmentDiv
              | expression '%=' expression   # assignmentMod
              | expression '+=' expression   # assignmentPlus
              | expression '-=' expression   # assignmentMinus
              | expression '<<=' expression  # assignmentShiftLeft
              | expression '>>=' expression  # assignmentShiftRight
              | expression '&=' expression   # assignmentBitAnd
              | expression '^=' expression   # assignmentBitXor
              | expression '|=' expression   # assignmentBitOr

              // Comma
              | expression ',' expression    # comma
              ;

STRING        : '"' (~["\r\n] | '""' | '\\"')* '"';
NUMBER        : [1-9][0-9_]* | '0';
IDENTIFIER    : [a-zA-Z_][a-zA-Z_0-9]*;

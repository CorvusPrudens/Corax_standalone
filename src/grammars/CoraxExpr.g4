grammar CoraxExpr;

// NOTE -- constant expressions will need to be identified through semantics
// TODO -- this needs to be consolidated as much as possible to avoid
// ridiculously long trees
              // Primary
expr_primary  : IDENTIFIER                   # identifier
              | NUMBER                       # const
              | STRING                       # string
              | '(' expression ')'           # group
              ;

              // postfix
expr_postfix  : expr_primary                       # exprPostfix         
              | expr_postfix '[' expression ']'    # indexing
              | expr_postfix '(' arg_expr_list ')' # call // (go to page 70)
              | expr_postfix '.' IDENTIFIER        # member
              | expr_postfix '->' IDENTIFIER       # indirectMember
              | expr_postfix '++'                  # incrementPost
              | expr_postfix '--'                  # decrementPost
              // TODO -- add this after declaration / initialization is settled
              // | '(' typename ')' '{' initializer ','? '}' #idk // (page 69)
              ;
              
              // Unary
expr_unary    : expr_postfix                 # exprUnary
              | '++' expr_unary              # incrementUnary
              | '--' expr_unary              # decrementUnary
              // TODO -- make these cast expressions!
              | '&' expression               # address
              | '*' expression               # dereference
              | '+' expression               # positive 
              | '-' expression               # negative
              | '~' expression               # not
              | '!' expression               # negation
              | 'sizeof' expr_unary          # sizeof
              | 'sizeof' '(' typename ')'    # sizeofType
              ;

expr_cast     : expr_unary                   # exprCast
              | '(' typename ')' expr_cast   # cast
              ;

              // Multiplicative
expr_multi    : expr_cast                    # exprMulti
              | expr_multi '*' expr_multi    # mult
              | expr_multi '/' expr_multi    # div
              | expr_multi '%' expr_multi    # mod
              ;

              // Additive
expr_addi     : expr_multi                   # exprAddi
              | expr_addi '+' expr_addi      # plus
              | expr_addi '-' expr_addi      # minus
              ;

              // Bitwise shift
expr_shift    : expr_addi                    # exprShift
              | expr_shift '<<' expr_shift   # shiftLeft
              | expr_shift '>>' expr_shift   # shiftRight
              ;

              // Relational
expr_relation : expr_shift                       # exprRelation
              | expr_relation '<' expr_relation  # less
              | expr_relation '>' expr_relation  # greater
              | expr_relation '<=' expr_relation # less_equal
              | expr_relation '>=' expr_relation # greater_equal
              ;

              // Equality
expr_equality : expr_relation                # exprEquality
              | expr_equality '==' expr_equality   # equal
              | expr_equality '!=' expr_equality   # notEqual
              ;

              // Bitwise
expr_bitwise  : expr_equality                # exprBitwise
              | expr_bitwise '&' expr_bitwise    # bit_and
              | expr_bitwise '^' expr_bitwise    # bit_xor
              | expr_bitwise '|' expr_bitwise    # bit_or
              ;

              // Logical
expr_logical  : expr_bitwise                # exprLogical
              | expr_logical '&&' expr_logical   # and
              | expr_logical '||' expr_logical   # or
              ;

              // Ternary
expr_tern     : expr_logical                     # exprTern
              | expr_logical '?' expression ':' expr_tern # ternary
              ;

expr_assi     : expr_tern                   # exprAssi
              | expr_unary '=' expr_assi    # assignment
              | expr_unary '*=' expr_assi   # assignmentMult
              | expr_unary '/=' expr_assi   # assignmentDiv
              | expr_unary '%=' expr_assi   # assignmentMod
              | expr_unary '+=' expr_assi   # assignmentPlus
              | expr_unary '-=' expr_assi   # assignmentMinus
              | expr_unary '<<=' expr_assi  # assignmentShiftLeft
              | expr_unary '>>=' expr_assi  # assignmentShiftRight
              | expr_unary '&=' expr_assi   # assignmentBitAnd
              | expr_unary '^=' expr_assi   # assignmentBitXor
              | expr_unary '|=' expr_assi   # assignmentBitOr
              ;

arg_expr_list : expr_assi                   # exprList
              | arg_expr_list ',' arg_expr_list # arglist
              ;

              // Comma
expression    : expr_assi                    # exprExpression
              | expression ',' expression    # comma
              ;

expr_const    : expr_tern;

typename      : IDENTIFIER+;

STRING        : '"' (~["\r\n] | '""' | '\\"')* '"';
NUMBER        : [1-9][0-9_]* | '0';
IDENTIFIER    : [a-zA-Z_][a-zA-Z_0-9]*;

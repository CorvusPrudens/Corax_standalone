grammar Corax;

import CoraxExpr;

// parsey

// This represents the translation unit
parse       : extern_decl+;

// TODO -- ensure this is all refactored into antlr-idiomatic code
declaration : decl_spec (init_decl (',' init_decl)* ',')? ';';

// Declaration specifiers
decl_spec   : storage_spec decl_spec?
            | type_spec decl_spec?
            | type_qual decl_spec?
            | func_spec decl_spec?
            ;

init_decl   : declarator
            | declarator '=' initializer
            ;

storage_spec : 'typedef'
             | 'extern'
             | 'static'
             | 'auto'
             | 'register'
             ;

// this kind of stuff should (probably?) be done with semantics
type_spec    : 'void'
             | 'char'
             | 'short'
             | 'int'
             | 'long'
             | 'float'
             | 'double'
             | 'signed'
             | 'unsigned'
             | '_Bool'
             | '_Complex'
             | struct_union
             | enum_spec
             | IDENTIFIER
             ;

// Is the difference between struct and union significant??
struct_union : 'struct' IDENTIFIER? '{' struct_decl '}'
             | 'struct' IDENTIFIER
             | 'union' IDENTIFIER? '{' struct_decl '}'
             | 'union' IDENTIFIER
             ;

struct_decl_list : struct_decl | struct_decl_list struct_decl;

struct_decl : spec_qual_list struct_decl_list ';';

spec_qual_list : type_spec spec_qual_list? | type_qual spec_qual_list?;

struct_declr_list : struct_declr | struct_declr_list ',' struct_declr;

struct_declr : declarator | declarator? ':' expr_const;

enum_spec    : 'enum' IDENTIFIER? '{' enumerator (',' enumerator)* ','? '}'
             | 'enum' IDENTIFIER
             ;

// is enum_const just an identifier??
enumerator   : IDENTIFIER
             | IDENTIFIER '=' expr_const
             ;

type_qual    : 'const'
             | 'restrict'
             | 'volatile'
             ;

// can probably be factored out
func_spec    : 'inline';

declarator   : pointer? direct_decl;

direct_decl  : IDENTIFIER
             | '(' declarator ')'
             | direct_decl '[' type_qual* expr_assi? ']'
             | direct_decl '[' 'static' type_qual* expr_assi ']'
             | direct_decl '[' type_qual+ 'static' expr_assi ']'
             | direct_decl '[' type_qual* '*' ']'
             | direct_decl '(' param_type_list ')'
             | direct_decl '(' (IDENTIFIER (',' IDENTIFIER)* ','?)? ')'
             ;

pointer : ('*' type_qual*)+
        // | '*' type_qual* pointer
        ;

param_type_list: param_list ','? | param_list ',' '...';

param_list: param_decl (',' param_decl)*;

param_decl : decl_spec declarator
           | decl_spec abstract_decl?
           ;

typename      : spec_qual_list abstract_decl?;

// abstract_decl : pointer 
//               | pointer? direct_abstr
//               ;

abstract_decl : pointer
              | pointer? abstract_decl
              | '(' abstract_decl ')' 
              | abstract_decl? '[' type_qual* expr_assi? ']'
              | abstract_decl? '[' 'static' type_qual* expr_assi ']'
              | abstract_decl? '[' type_qual* 'static' expr_assi ']'
              | abstract_decl? '[' '*' ']'
              | abstract_decl? '(' param_type_list ')'
              ;

initializer   : expr_assi
              | '{' init_list '}'
              ;

init_list     : designation? initializer (',' designation? initializer)* ','?;

designation   : designator+ '=';

designator    : '[' expr_const ']'
              | '.' IDENTIFIER
              ;

statement     : stat_labeled
              | stat_compound
              | stat_expr
              | stat_select
              | stat_iter
              | stat_jump
              ;

stat_labeled  : IDENTIFIER ':' statement
              | 'case' expr_const ':' statement
              | 'default' ':' statement
              ;

stat_compound : '{' block_item+ '}';

block_item    : declaration
              | statement
              ;

stat_expr     : expression? ';';

stat_select   : 'if' '(' expression ')' statement
              | 'if' '(' expression ')' statement 'else' statement
              | 'switch' '(' expression ')' statement
              ;

stat_iter     : 'while' '(' expression ')' statement
              | 'do' statement 'while' '(' expression ')' ';'
              | 'for' '(' expression? ';' expression? ';' expression? ')' statement
              | 'for' '(' declaration expression? ';' expression? ')' statement
              ;

stat_jump     : 'goto' IDENTIFIER ';'
              | 'continue' ';'
              | 'break' ';'
              | 'return' expression? ';'
              ;

extern_decl   : func_def | declaration;

func_def      : decl_spec declarator declaration* stat_compound;

// lexy

STRING                 : '"' (~["\r\n] | '""' | '\\"')* '"';
CHAR                   : '\'' (('\\'.)|~[\\]) '\'';
COMMENT                : '//' ~[\n\r]* [\n\r] -> skip;
COMMENT_BLOCK          : '/*' .*? '*/' -> skip;
WHITESPACE             : [ \t\n\r] -> skip;
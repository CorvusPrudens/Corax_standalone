grammar CppHash;

parse: ANYTHING* EOF;



COMMENT       : '//' ~[\n\r]* [\n\r] -> skip;
COMMENT_BLOCK : '/*' .*? '*/' -> skip;
WHITESPACE    : [ \t\n\r]+ -> skip;
ANYTHING      : ~[ \t\n\r]+?;
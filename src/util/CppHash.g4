grammar CppHash;

parse: anything* EOF;

anything: ANYTHING {self.md5.update($text.encode('utf-8'))};


COMMENT       : '//' ~[\n\r]* [\n\r] -> skip;
COMMENT_BLOCK : '/*' .*? '*/' -> skip;
WHITESPACE    : [ \t\n\r]+ -> skip;
ANYTHING      : ~[ \t\n\r]+?;
grammar Cp_post;

import Cp;

// Technically, these shouldn't be present after the
// precompiler runs, but this makes testing easy
PRECOMPILER            : '#' .*? [\n\r] -> skip;
WHITESPACE             : [ \t\n\r] -> skip;
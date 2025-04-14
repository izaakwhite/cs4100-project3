%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "tree_builder.tab.h"  

extern int line_number;
%}

%option nounput

%%
"buildnode"             { return BUILDNODE; }
"name"                  { return NAME; }
"weight"                { return WEIGHT; }
"for"                   { return FOR; }
"in"                    { return IN; }
"isachildof"            { return ISACHILDOF; }

\"([^\\\n]|(\\.))*?\"   { yylval.str_val = strdup(yytext); return STRING; }

[0-9]+                  { yylval.int_val = atoi(yytext); return INT; }

[A-Za-z][A-Za-z0-9_]*   { yylval.str_val = strdup(yytext); return VAR; }

"="                     { return ASSIGN; }
"+"                     { return PLUS; }

"{"                     { return LBRACE; }
"}"                     { return RBRACE; }
"["                     { return LBRACKET; }
"]"                     { return RBRACKET; }
";"                     { return SEMICOLON; }
":"                     { return COLON; }

\n                      { line_number++; }
[ \t\r]+                { /* ignore whitespace */ }

.                       { printf("Unknown character: %s\n", yytext); }
%%

int yywrap() {
    return 1;
}
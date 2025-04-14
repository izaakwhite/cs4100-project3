%code requires {
    #include <vector>
    #include "tree_node.h"
}

%{
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cstdlib>
#include <cstring> 
#include "tree_node.h"
#include "parse_tree.h"

extern int yylex();
extern FILE *yyin;
void yyerror(const char *s);
int line_number = 1;

%}

%token BUILDNODE NAME WEGIHT FOR IN ISACHILDOF


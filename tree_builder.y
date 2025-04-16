%code requires {
    #include "parse_tree.h"
}

%start program
%token BUILDNODE NAME WEIGHT FOR IN ISACHILDOF STRING INT VAR

%{
#include <iostream>
#include <string>
#include <cstring>
#include "print_tree.h"

using namespace std;

extern int yylex();
extern void yyerror(const char *s);
int line_number = 1;

%}

%union {
    int int_val;
    char* str_val;
    Expression* expr_val;
    Statement* stmt_val;
    CompoundStatement* comp_stmt_val;
    Program* program_val;
}

%{
    Program* program_root = nullptr;
%}

%type <int_val> INT
%type <str_val> STRING VAR
%type <expr_val> expression string_expression
%type <stmt_val> statement buildnode_statement for_statement buildnode_params
%type <comp_stmt_val> statement_list block
%type <program_val> program

%%

program: statement_list {
    $$ = new Program($1);
    program_root = $$;
}
;

statement_list:  {
    $$ = new CompoundStatement();
    }
| statement_list statement {
    $1->addStatement($2); $$ = $1;
}
;

statement: buildnode_statement { $$ = $1; }
| for_statement { $$ = $1; }
;

buildnode_statement: BUILDNODE '{' buildnode_params '}' ';' {
    $$ = $3;
}
;

buildnode_params: NAME '=' string_expression ';' WEIGHT '=' expression ';' {
    $$ = new BuildNodeStatement($3, $7);
} 
| NAME '=' string_expression ';' WEIGHT '=' expression ';' ISACHILDOF '=' string_expression ';' {
    $$ = new BuildNodeStatement($3, $7, $11);
}
;

for_statement: FOR VAR IN '[' expression ':' expression ']' block {
    $$ = new ForStatement($2, $5, $7, $9);
    free($2); 
}
;

block: '{' statement_list '}' {
    $$ = $2;
}
;

expression: INT {
    $$ = new IntLiteral($1);
}
| VAR {
    $$ = new Variable($1);
    free($1); 
}
| expression '+' expression {
    $$ = new AddExpr($1, $3);
}
| '(' expression ')' {
    $$ = $2;
}
;

string_expression: STRING {
    $$ = new StringLiteral($1);
    free($1); 
}
| VAR {
    $$ = new Variable($1);
    free($1);
}
| string_expression '+' expression {
    $$ = new AddExpr($1, $3);
}
;

%%

void yyerror(const char *s) {
    cerr << "Error: " << s << " at line " << line_number << endl;
    exit(1);
}

int main() {
    line_number = 1;

    if (yyparse() == 0) {
        if (program_root) {
            program_root->execute();
  
            TreeNode* root = program_root->getRoot();
            if (root) {
                cout << "=== Tree Structure ===\n";
                printTree(root);
                cout << endl;
                // printTree_Weights(root);
            }
            delete program_root;
        }
    }
    
    return 0;
}


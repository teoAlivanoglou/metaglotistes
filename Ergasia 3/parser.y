%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int flag=0;
int line=1;
extern char *yytext;
#define YYSTYPE char *
%}

%token KEYWORDS
%token OPERATOR
%token IDENTIFIER
%token STRING
%token BININTEGER
%token INTEGER
%token FLOATNUMBER
%token IMAGINARYNUMBER
%token ID_ERR STRING_ERR BIN_ERR1 INT_ERR1 INT_ERR2 INT_ERR3 FLOAT_ERR_POINT FLOAT_ERR_SIGN FLOAT_ERR_LETTER

%start program

%% 

program : program atom 
	| program list
	| program simple_stm
	| program user_fun
	| program arith_stm
	| program assignment_stm
	| program if_stm
	| program while_stm
	|

atom 		: id | literal | list 		{ printf(" Line %d: Atom found\n", line); }

list		: '[' starred_expression ']'    {printf(" Line %d: A list is declared\n", line);}

id		: IDENTIFIER { $$ = strdup(yytext); } 

literal		: STRING | INTEGER | FLOATNUMBER | IMAGINARYNUMBER | BININTEGER //{ $$ = strdupyytext); }

starred_expression : starred_item | starred_item ',' starred_item

starred_item	: id | literal


simple_stm 	: del_stm | len_stm | cmp_stm | print_stm	{printf("\tLine %d: Simple statement found!\n", line);}

del_stm		: "del("  id ")"				{printf("\t\tLine %d: del statement found!\n", line);}

len_stm		: "len(" list ")"				{printf("\t\tLine %d: len statement found!\n", line);}

cmp_stm		: "cmp(" list | id "," list|id ")"		{printf("\t\tLine %d: cmp statement found!\n", line);}

print_stm	: "print(" atom ")" | "print" cmp_stm		{printf("\t\tLine %d: print statement found!\n", line);}

user_fun	: "def" id "(" parameter "):" "\n" STRING whatever "\n" "return" id   {printf("\tLine %d: Function declared!\n", line);}
parameter	: id | id "," id
whatever	: arith_stm | simple_stm | if_stm | while_stm 

arith_stm 	: m_expr | a_expr	{printf("\t\tLine %d: Arithmetic statement found!\n", line);}
m_expr		: u_expr | m_expr "*" u_expr | m_expr "/" u_expr
a_expr		: m_expr | a_expr "+" m_expr | a_expr "-" m_expr
u_expr		: id 	 | literal 	     | "-" u_expr       | "+" u_expr

assignment_stm  : target_list "=" target	{printf("\tLine %d: Assignment found!\n", line);}
target_list	: id | id "," id
target		: literal

if_stm		: "if" expression ":" suite	{printf("\tLine %d: 'if' statement found!\n", line);}
expression	: id ">" id | id "<" id | id "==" id | id "!=" id | id ">" literal | id "<" literal | id "==" literal | id "!=" literal
suite		: whatever

while_stm	: "while" expression ":" suite {printf("\tLine %d: 'while' statement found!\n", line);}
;
%%

int yyerror(void)
{}

FILE *yyin;

int main(int argc,char **argv) 
{
	int i;
	if(argc == 2)
		yyin=fopen(argv[1],"r");
	else
		yyin=stdin;

	int parse = yyparse();

	if (parse==0)
		{
			printf("\nINPUT FILE: PARSING SUCCEEDED.\n");
			yyparse();
		}
	else
		{
			printf("\nINPUT FILE: PARSING FAILED.\n");
			yyparse();
		}

	return 0;
}

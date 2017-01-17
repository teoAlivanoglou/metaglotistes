%option noyywrap

%{ 
#include "parser.tab.h"
//#include "token.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern int flag;
extern int line;
%}

DELIMITER	 [ ]+|[/t]+

KEYWORDS		 False|class|finally|is|return|None|continue|for|lambda|try|True|def|from|nonlocal|while|and|del|global|not|with|as|elif|if|or|yield|assert|else|import|pass|break|except|in|raise

OPERATOR	 ([+\-$@&|^~])|(\*{1,2})|(\/{1,2})|(<[=<]?)|(>[=>]?)|(==)|!

COMMENT		 #.+

IDENTIFIER	 [a-zA-Z_][a-zA-Z0-9_]*
ID_ERR		 [0-9]{IDENTIFIER}

STRINGESCAPESEQ	 [\\].	
SHORTSTRINGCHAR1 [^\\'\n]*
SHORTSTRINGCHAR2 [^\\"\n]*
SHORTSTRINGITEM1 {SHORTSTRINGCHAR1}|{STRINGESCAPESEQ}
SHORTSTRINGITEM2 {SHORTSTRINGCHAR2}|{STRINGESCAPESEQ}
STRING		 [']{SHORTSTRINGITEM1}*[']|(["]{SHORTSTRINGITEM2}*["])
STRING_ERR	 ['].[^'\n\t]*|["].[^"\n\t]* 

BINDIGIT	 [01]
BININTEGER	 0[bB]{BINDIGIT}+
BIN_ERR1	 0[Bb]+{BINDIGIT}+

DIGIT		 [0-9]
NONZERODIGIT	 [1-9]
INTEGER		 {NONZERODIGIT}{DIGIT}*|[0]+
INT_ERR1	 {INTEGER}[a-zA-Z]{DIGIT}
INT_ERR2	 [-+]{INTEGER}
INT_ERR3	 [0+]{INTEGER} 

INTPART		 {DIGIT}+
EXPONENT	 [eE][+-]*{INTPART}
FRACTION	 [\.]{INTPART}
EXPONENTFLOAT	 ({INTPART}|{POINTFLOAT}){EXPONENT}
POINTFLOAT	 {INTPART}*{FRACTION}|{INTPART}[\.]
FLOATNUMBER	 {POINTFLOAT}|{EXPONENTFLOAT}
FLOAT_ERR_POINT	 {INTPART}*[\.]+{DIGIT}+[\.]*{DIGIT}*|{INTPART}[\.]+
FLOAT_ERR_SIGN	 ([-+]{FLOATNUMBER})
FLOAT_ERR_LETTER {FLOATNUMBER}[A-Za-z]+ 

IMAGINARYNUMBER  ({FLOATNUMBER}|{INTPART})[jJ]

%%

{DELIMITER}		{ }
{KEYWORDS}		{ return KEYWORDS; }
{OPERATOR}		{ }
{COMMENT}		{ printf("Line %d: Found COMMENT\n", line); }	
{IDENTIFIER}		{ printf("Line %d: Found IDENTIFIER %s\n", line, yytext);					 return IDENTIFIER; }
{ID_ERR}		{ printf("Line %d: !!ERROR!!\tWrong IDENTIFIER statement %s\n", line, yytext); 			 return ID_ERR;}  
{STRING}		{ printf("Line %d: Found STRING %s\n", line, yytext);						 return STRING; }
{STRING_ERR}		{ printf("Line %d: !!ERROR!!\tWrong STRING statement %s\n", line, yytext); 			 return STRING_ERR;}
{BININTEGER}		{ printf("Line %d: Found BINARY INTEGER NUMBER %s\n", line, yytext);				 return BININTEGER; }
{BIN_ERR1}		{ printf("Line %d: !!ERROR!!\tWrong BINARY NUMBER statement, too many 'Bb's%s\n", line, yytext); return BIN_ERR1;} 
{INTEGER}		{ printf("Line %d: Found INTEGER NUMBER %s\n", line, yytext);					 return INTEGER; }
{INT_ERR1}		{ printf("Line %d: !!ERROR!!\tWrong INT statement %s\n", line, yytext); 			 return INT_ERR1;}
{INT_ERR2}		{ printf("Line %d: !!ERROR!!\tWrong INT statement, '+/-' found %s\n", line, yytext); 		 return INT_ERR2;}
{INT_ERR3}		{ printf("Line %d: !!ERROR!!\tWrong INT statement, first digit(s) zeros %s\n", line, yytext); 	 return INT_ERR3;} 
{FLOATNUMBER}		{ printf("Line %d: Found FLOAT NUMBER %s\n", line, yytext);				  	 return FLOATNUMBER; }
{FLOAT_ERR_POINT}	{ printf("Line %d: !!ERROR!!\tWrong FLOAT statement, too many '.' %s\n", line, yytext); 	 return FLOAT_ERR_POINT;}
{FLOAT_ERR_SIGN}	{ printf("Line %d: !!ERROR!!\tWrong FLOAT statement, '+/-' FOUND %s\n", line, yytext); 		 return FLOAT_ERR_SIGN;}
{FLOAT_ERR_LETTER}	{ printf("Line %d: !!ERROR!!\tWrong FLOAT statement, letter FOUND %s\n", line, yytext); 	 return FLOAT_ERR_LETTER;}
{IMAGINARYNUMBER}	{ printf("Line %d: Found IMAGINARY NUMBER %s\n", line, yytext);					 return IMAGINARYNUMBER; } 
. 			{ printf("Line %d: UNKNOWN TOKEN:%s", line, yytext); }
\n              	{ line++; }
<<EOF>>			{ printf("#END OF FILE\n"); exit(0); }

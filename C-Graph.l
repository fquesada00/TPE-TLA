%{
    #include "y.tab.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    void yyerror (char *s);
    int yylex();
%}
digits [0-9]+
alpha [a-zA-Z]{1,256}
ascii-printables [ -~]{1,256}
ignore [ \t\n]+
entrypoint graph
%% 
(==|!=|>=|<=|>|<|&&|\|\|) {yylval.string = strdup(yytext);return BINARY_BOOL_OPERATOR;}
!                         {yylval.string = strdup(yytext);return UNARY_BOOL_OPERATOR;}
[=;{}\(\)]                {return yytext[0];}   
[+\*/-]                   {yylval.string = strdup(yytext);return OPERATOR;}
int                       {return INT;}
else-if                   {return ELSE_IF;}
if                        {return IF;}
else                      {return ELSE;}
string                    {return STRING;}
{entrypoint}              {return GRAPH;}
\"{ascii-printables}\"    {yylval.string = strdup(yytext);return STRING_VALUE;}
{alpha}                   {yylval.string = strdup(yytext);return ID;}
{digits}                  {yylval.num = atoi(yytext);return NUMBER;}
{ignore}                  ;
.                         ;
%%
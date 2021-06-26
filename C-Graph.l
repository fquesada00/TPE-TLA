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
[=;{}\(\)]           {return yytext[0];}
[+\*/-]                   {yylval.string = strdup(yytext);return OPERATOR;}
int                       {return INT;}
string                    {return STRING;}
{entrypoint}              {return GRAPH;}
\"{ascii-printables}\"    {yylval.string = strdup(yytext);return STRING_VALUE;}
{alpha}                   {yylval.string = strdup(yytext);return ID;}
{digits}                  {yylval.num = atoi(yytext);return NUMBER;}
{ignore}                  ;
.                         ;
%%
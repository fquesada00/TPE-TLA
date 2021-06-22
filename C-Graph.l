%{
    #include "y.tab.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "C-Graph.h"
    void yyerror (char *s);
    int yylex();
%}
digits [0-9]+
alpha [a-zA-Z]{1,256}
ignore [ \t\n]+
entrypoint graph\(\)
%%
int                       {yylval.type = INT; return identifier_type;}
string                    {yylval.type = STRING; return identifier_type;}
{entrypoint}              {printf("Starting program...\n");return graph;}
{alpha}                   {yylval.string = yytext; return identifier;}
{digits}                  {yylval.integer = atoi(yytext);return number;}
[+-=;{}]                  {return yytext[0];}
{ignore}                  ;
.                         ;
%%
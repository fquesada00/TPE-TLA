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
entrypoint graph
%%
[+=;{}\(\)-]              {puts("here1");return yytext[0];}
int                       {puts("here2");yylval.type = INT; return identifier_type;}
string                    {puts("here3");yylval.type = STRING; return identifier_type;}
{entrypoint}              {printf("Starting program....\n");return graph;}
{alpha}                   {puts("here4");yylval.string = yytext; return identifier;}
{digits}                  {puts("here5");yylval.integer = atoi(yytext);return number;}
{ignore}                  {puts("here6");}
.                         {puts("here7");}
%%
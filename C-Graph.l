%{
    #include "y.tab.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    void yyerror (char *s);
    int yylex();
%}
digits [0-9]+
alpha [a-zA-Z]{1,240}[0-9]{0,16}
ascii-printables [ -!#-~]{1,256}
comment-printables [ -~]{1,256}
ignore [ \t]+
enter [\n]
entrypoint graph
%% 
@{comment-printables}\n   {return COMMENT;}
--\/->                    {return REMOVE_ADJACENCY;}
\/>                       {return REMOVE_NODE;}
->                        {yylval.string = strdup(yytext); return END_ARROW;}
--                        {yylval.string = strdup(yytext);return START_ARROW;}
(==|!=|>=|<=|>|<|&&|\|\|) {yylval.string = strdup(yytext);return BINARY_BOOL_OPERATOR;}
!                         {yylval.string = strdup(yytext);return UNARY_BOOL_OPERATOR;}
[=;{}\(\),]               {return yytext[0];}   
[+\*/-]                   {yylval.string = strdup(yytext);return OPERATOR;}
int                       {return INT;}
for                       {return FOR;}
input                     {return INPUT;}
output                    {return OUTPUT;}
else-if                   {return ELSE_IF;}
if                        {return IF;}
else                      {return ELSE;}
string                    {return STRING;}
node                      {return NODE_ITERATOR;}
edge                      {return EDGE_ITERATOR;}
foreach                   {return FOREACH;}
\.value                   {return NODE_VALUE;}
\.id                      {return NODE_ID;}
\.weight                  {return EDGE_WEIGHT;}
Node                      {return NODE;}
in                        {yylval.string = strdup(yytext);return IN;}
traverse                  {return TRAVERSE;}
starting                  {return STARTING;}
with                      {return WITH;}
dfs                       {return DFS;}
bfs                       {return BFS;}
{entrypoint}              {return GRAPH;}
\"{ascii-printables}\"    {yylval.string = strdup(yytext);return STRING_VALUE;}
{alpha}                   {yylval.string = strdup(yytext);return ID;}
{enter}                   {yylineno++;}
{digits}                  {yylval.num = atoi(yytext);return NUMBER;}
{ignore}                  ;
.                         ;
%%
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include "AST.h"

typedef struct Symbol{
    char * name;
    AstDeclarationType dataType;
    struct Symbol * next; 
}Symbol;
typedef struct SymbolTable{
    Symbol * first;
    Symbol * last;
    struct SymbolTable * prev;
    int size;
}SymbolTable;

typedef struct ScopeTable{
    SymbolTable * top;
    int size;
}ScopeTable;

void freeSymbolTable(SymbolTable * symbolTable);
void pushScope(ScopeTable * scopeTable);
void popScope(ScopeTable * scopeTable);
Symbol * findSymbolInTable(SymbolTable * symbolTable,char * name);
Symbol * findSymbol(ScopeTable * scopeTable,char * name);
void addSymbolToTable(SymbolTable * symbolTable,char * name, AstDeclarationType dataType);
int addSymbol(ScopeTable * scopeTable,char * name, AstDeclarationType dataType);
ScopeTable * createScopeTable();


#endif
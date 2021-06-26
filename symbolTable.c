#include <string.h>
#include <stdlib.h>
#include "symbolTable.h"
#define MAX_STRING_LEN 256

void freeSymbolTable(SymbolTable * symbolTable){
    Symbol *curr,* it = symbolTable->first;
    while(it != NULL){
        free(it->name);
        curr = it;
        it = it->next;
        free(curr);
    }
    free(symbolTable);
}

void pushScope(ScopeTable * scopeTable){
    SymbolTable * new = malloc(sizeof(SymbolTable));
    new->first = new->last = NULL;
    new->size = 0;
    new->prev = scopeTable->top;
    scopeTable->top = new;
    scopeTable->size++;
}

void popScope(ScopeTable * scopeTable){
    if(scopeTable == NULL || scopeTable->top == NULL){
        return;
    }
    SymbolTable * remove = scopeTable->top;
    scopeTable->top = scopeTable->top->prev;
    freeSymbolTable(remove);
    scopeTable->size--;
}


Symbol * findSymbolInTable(SymbolTable * symbolTable,char * name){
    Symbol * it = symbolTable->first;
    while(it != NULL){
        if(strncmp(it->name,name,MAX_STRING_LEN) == 0)  return it;
        it = it->next;
    }
    return NULL;
}

Symbol *  findSymbol(ScopeTable * scopeTable,char * name){
    SymbolTable * it = scopeTable->top;
    Symbol * ret = NULL;
    while (it != NULL)
    {
        if((ret=findSymbolInTable(it,name)) != NULL)
            break;
        it = it->prev;
    }
    return ret;
}

void addSymbolToTable(SymbolTable * symbolTable,char * name, AstDeclarationType dataType){
    
    Symbol * new = malloc(sizeof(Symbol));
    new->next = NULL;
    int len = strnlen(name,MAX_STRING_LEN);
    new->name = malloc((len + 1) * sizeof(char));
    strncpy(new->name,name,len + 1);
    new->dataType = dataType;
    if(symbolTable->size == 0){
        symbolTable->first = new;
    }else{
        symbolTable->last->next = new;
    }
    symbolTable->last = new;
    symbolTable->size++;

}

int addSymbol(ScopeTable * scopeTable,char * name, AstDeclarationType dataType){
    if(scopeTable == NULL || scopeTable->top == NULL)
        return -1;
    
    addSymbolToTable(scopeTable->top,name, dataType);
    return 0;
}

ScopeTable * createScopeTable() {
    return calloc(1, sizeof(ScopeTable));
}
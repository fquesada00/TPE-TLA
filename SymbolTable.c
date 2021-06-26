#include <string.h>
#define MAX_STRING_LEN 256
typedef struct Symbol{
    char * name;
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
    SymbolTable * new = malloc(sizeof(SymbolTable));;
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


int findSymbolInTable(SymbolTable * symbolTable,char * name){
    Symbol * it = symbolTable->first;
    while(it != NULL){
        if(strcmp(it->name,name) == 0)  return 1;
        it = it->next;
    }
    return 0;
}

int findSymbol(ScopeTable * scopeTable,char * name){
    SymbolTable * it = scopeTable->top;
    while (it != NULL)
    {
        if(findSymbolInTable(it,name))
            return 1;
        it = it->prev;
    }
    return 0;
}

void addSymbolToTable(SymbolTable * symbolTable,char * name){
    
    Symbol * new = malloc(sizeof(Symbol));
    new->next = NULL;
    if(symbolTable->size == 0){
        symbolTable->first = new;
    }else{
        symbolTable->last->next = new;
    }
    symbolTable->last = new;
    symbolTable->size++;

}

int addSymbol(ScopeTable * scopeTable,char * name){
    if(scopeTable == NULL || scopeTable->top == NULL)
        return -1;
    
    addSymbolToTable(scopeTable->top,name);
}
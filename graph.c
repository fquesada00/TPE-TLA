#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define START_SIZE 32
#define STR_MAX_LEN 64

typedef enum edgeType{INT_EDGE,STRING_EDGE,CHAR_EDGE}edgeType;

typedef struct GraphEdge{
    edgeType type;
    void * weight;
}GraphEdge;

typedef struct GraphNode{
    struct GraphNodeListElement * lastNode;
    struct GraphNodeListElement * firstNode;
    struct GraphEdgeListElement * lastEdge;
    struct GraphEdgeListElement * firstEdge;
    size_t size;
}GraphNode;


typedef struct Graph{
    // GraphNode * root;
    struct GraphNodeListElement * last;
    struct GraphNodeListElement * first;
    size_t size;
}Graph;

// nodos del grafo por orden de insercion
typedef struct GraphNodeListElement{
    GraphNode * value;
    struct GraphNodeListElement * next;
}GraphNodeListElement;


typedef struct GraphEdgeListElement{
    GraphEdge * value;
    struct GraphEdgeListElement * next;
}GraphEdgeListElement;

Graph * graph;

Graph * newGraph(){
    Graph * g = calloc(1,sizeof(Graph));
    return g;
}

/* private */ void addGraphNode(GraphNode * node){
    if(graph->size == 0){
        graph->first = calloc(1,sizeof(GraphNodeListElement));
        graph->first->value = node;
        graph->first->next = NULL;
        graph->last = graph->first;
        graph->size++;
        return;
    }
    graph->last->next = calloc(1,sizeof(GraphNodeListElement));
    graph->last->next->value = node;
    graph->last->next->next=NULL;
    graph->last = graph->last->next;
    graph->size++;
}

GraphNode * newGraphNode(){
    GraphNode * n = calloc(1,sizeof(GraphNode));
    n->lastNode = calloc(1,sizeof(GraphNodeListElement));
    n->firstNode = calloc(1,sizeof(GraphNodeListElement));
    n->lastEdge = calloc(1, sizeof(GraphEdgeListElement));
    n->firstEdge = calloc(1, sizeof(GraphEdgeListElement));
    addGraphNode(n);
    return n;
}

/* private */ GraphEdge * newEdge(void * value,edgeType type){
    GraphEdge * e = calloc(1,sizeof(GraphEdge));
    int size = 0;
    switch (type){
    case INT_EDGE:
        size = sizeof(int);
        break;
    case STRING_EDGE:
        size = sizeof(char) * strnlen((char *)value,STR_MAX_LEN);
        break;
    case CHAR_EDGE:
        size = sizeof(char);
        break;
    
    default:
        break;
    }
    e->type = type;
    e->weight = malloc(size);
    memcpy(e->weight,value,size);
    return e;
}

bool isAdjacent(GraphNode * origin, GraphNode * destination) {
    
    for(GraphNodeListElement * n = origin->firstNode; n != NULL; n = n->next){
        if(n->value == destination) {
            return true;
        }
    }
    return false;
}

void addEdge(GraphNode * node1, GraphNode * node2, void * value, edgeType type){
    if(node1->size ==0){
        node1->firstEdge = calloc(1,sizeof(GraphEdgeListElement));
        node1->firstEdge->value = newEdge(value,type);
        node1->firstNode->next=NULL;
        node1->lastEdge = node1->firstEdge;
        node1->firstNode = calloc(1,sizeof(GraphNodeListElement));
        node1->firstNode->value = node2;
        node1->firstNode->next = NULL;
        node1->lastNode = node1->firstNode;
        node1->size++;
        return;
    }
    if(isAdjacent(node1,node2)){
        return;
    }
    node1->lastEdge->next = calloc(1,sizeof(GraphEdgeListElement));
    node1->lastEdge->value = newEdge(valloc,type);
    node1->lastEdge->next->next = NULL;
    node1->lastEdge = node1->lastEdge->next;
    node1->lastNode->next = calloc(1,sizeof(GraphNodeListElement));
    node1->lastNode->next->value = node2;
    node1->lastNode->next->next = NULL;
    node1->lastNode = node2;
    node1->size++;
}

void setRoot(GraphNode * node){
    // graph->root = node;
}

/* private */ void freeEdge(GraphEdge * edge){
    free(edge->weight);
    free(edge);
}

/* private */ void freeEdges(GraphNode * node){
    GraphEdgeListElement * e = node->firstEdge;
    GraphNodeListElement * n = node->firstNode;
    while(e != NULL || n != NULL){
        freeEdge(e->value);
        GraphEdgeListElement * currE = e;
        GraphNodeListElement * currN = n;
        e = e->next;
        n = n->next;
        free(currE);
        free(currN);
    }
}

void removeEdge(GraphNode * node1, GraphNode * node2) {
    GraphEdgeListElement * e = node1->firstEdge;
    GraphNodeListElement * n = node1->firstNode;
    GraphEdgeListElement * prevE = NULL;
    GraphNodeListElement * prevN = NULL;

    if(n != NULL && n->value == node2){
        node1->firstEdge = e->next;
        node1->firstNode = n->next;
        freeEdge(e->value);
        free(e);
        free(n);
        return;
    }
    while(n != NULL && n->value != node2) {
        prevE = e;
        prevN = n;
        e = e->next;
        n = n->next;
    }
    if(n == NULL)
        return;
    prevN->next = n->next;
    prevE->next = e->next;
    freeEdge(n->value);
    free(n);
    free(e);
}
/*
    * Hay que recorrer todos los nodos del grafo menos el que quiero eliminar y por cada uno eliminar su adyacencia
*/
/* private */ void removeAdjacency(GraphNode * node){
    GraphNodeListElement * n = graph->first;
    while(n != NULL){
        if(n->value != node){
            removeEdge(n->value,node);
        }
        n = n->next;
    }
}

void removeNode(GraphNode * node){
    GraphNodeListElement * n = graph->first;
    GraphNodeListElement * prevN = NULL;
    if(n != NULL && n->value == node){
        graph->first = n->next;
        removeAdjacency(n);
        free(n);
        return;
    }

    while(n != NULL && n->value != node){
        prevN = n;
        n = n->next;
    }
    if(n == NULL)
        return;
    prevN->next = n->next;
    removeAdjacency(n);
    free(n);
}

int main(){
    graph = newGraph();
    //Node node1, node2, node3, node4, node5
    GraphNode * node1 = newGraphNode();
    GraphNode * node2 = newGraphNode();
    GraphNode * node3 = newGraphNode();
    GraphNode * node4 = newGraphNode();
    GraphNode * node5 = newGraphNode();
    //node1-'h'->node2;
    char a = 'h';//-->Necesary?
    addEdge(node1,node2,(void *)&a,CHAR_EDGE);
    //node2-'o'->node3->node4;
    char b = 'h';
    char c = 'l';
    addEdge(node2,node3,(void *)&b,CHAR_EDGE);
    addEdge(node3,node4,(void *)&c,CHAR_EDGE);
    //node4-'a'->node5;
    char d = 'a';
    addEdge(node4,node5,(void *)&d,CHAR_EDGE);
    //node5-' '->node1;
    char e = ' ';
    addEdge(node5,node1,(void *)&e,CHAR_EDGE);
    //->node5;
    setRoot(node5);
    //destroy node5;
    removeNode(node5);
    //foreach(node in graph)



}
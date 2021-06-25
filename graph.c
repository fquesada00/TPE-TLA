#include <stdio.h>
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
    int id;
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

void breakpoint() {
    int *p = NULL;
    printf("%d\n",*p);
}

Graph * newGraph(){
    Graph * g = calloc(1,sizeof(Graph));
    return g;
}

/* private */ void addGraphNode(Graph * graph, GraphNode * node){
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

GraphNode * newGraphNode(Graph * graph, int id){
    GraphNode * n = calloc(1,sizeof(GraphNode));
    n->firstNode = calloc(1,sizeof(GraphNodeListElement));
    n->lastNode = n->firstNode;
    n->firstEdge = calloc(1, sizeof(GraphEdgeListElement));
    n->lastEdge = n->firstEdge;
    n->id = id;
    addGraphNode(graph, n);

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
        node1->firstEdge->value = newEdge(value,type);
        node1->firstNode->next=NULL;
        node1->lastEdge = node1->firstEdge;
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
    node1->lastEdge->next->value = newEdge(value,type);
    node1->lastEdge->next->next = NULL;
    node1->lastEdge = node1->lastEdge->next;
    node1->lastNode->next = calloc(1,sizeof(GraphNodeListElement));
    node1->lastNode->next->value = node2;
    node1->lastNode->next->next = NULL;
    node1->lastNode = node1->lastNode->next;
    node1->size++;
}

void setRoot(GraphNode * node){
    // graph->root = node;
}

/* private */ void freeEdge(GraphEdge * edge){
    free(edge->weight);
    free(edge);
    printf("at %s\n", __func__);
}

/* private */ void freeEdges(GraphNode * node){
    GraphEdgeListElement * e = node->firstEdge, * currE = NULL;
    GraphNodeListElement * n = node->firstNode, * currN = NULL;
    while(e != NULL || n != NULL){
        freeEdge(e->value);
        currE = e;
        currN = n;
        e = e->next;
        n = n->next;
        free(currE);
        free(currN);
    }
}

/*  
** al sacar una arista tengo q actualizar la lista del nodo y arista
*/
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
        printf("at %s\n", __func__);
        node1->size--;
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
    freeEdge(e->value);
    free(n);
    free(e);
    printf("at %s\n", __func__);
    node1->size--;
}
/*
    * Hay que recorrer todos los nodos del grafo menos el que quiero eliminar y por cada uno eliminar su adyacencia
*/
/* private */ void removeAdjacency(Graph * graph, GraphNode * node){
    GraphNodeListElement * n = graph->first;
    while(n != NULL){
        if(n->value != node){
            removeEdge(n->value,node);
        }
        n = n->next;
    }
}

void removeNode(Graph * graph, GraphNode * node){
    GraphNodeListElement * n = graph->first;
    GraphNodeListElement * prevN = NULL;
    if(n != NULL && n->value == node){
        if(n == graph->last) {
            graph->last = prevN;
        }
        removeAdjacency(graph, n->value);
        graph->first = n->next;
        graph->size--;
        freeEdges(n->value);
        free(n->value);
        free(n);
        printf("at %s\n", __func__);
        return;
    }

    while(n != NULL && n->value != node){
        prevN = n;
        n = n->next;
    }
    if(n == NULL){
        return;
    }
    if(n == graph->last) {
        graph->last = prevN;
    }
    removeAdjacency(graph,n->value);
    prevN->next = n->next;
    graph->size--;
    freeEdges(n->value);
    free(n->value);
    free(n);
    printf("at %s\n", __func__);
}

void freeGraph(Graph * graph){
    GraphNodeListElement * it = graph->first, * next;

    while(it != NULL){
        next = it->next;
        removeNode(graph, it->value);
        it = next;
    }   
    free(graph);
    printf("at %s\n", __func__);

}

void printNodeList(GraphNode * node) {
    GraphEdgeListElement * e = node->firstEdge;
    GraphNodeListElement * n = node->firstNode;
    while(e != NULL || n != NULL){
        printf("%d-%c->%d\n",node->id,*((char*)e->value->weight),n->value->id);
        e = e->next;
        n = n->next;
    }
    printf("\n\n");
}

void printGraph(Graph * graph) {
    GraphNodeListElement * n = graph->first;
    while(n != NULL){
        printf("\nPrinting adjacency for node %d\n", n->value->id);
        printNodeList(n->value);
        n = n->next;
    }
}


int main(){
    Graph * graph = newGraph();
    //Node node1, node2, node3, node4, node5
    GraphNode * node1 = newGraphNode(graph,1);
    GraphNode * node2 = newGraphNode(graph, 2);
    GraphNode * node3 = newGraphNode(graph,3);
    GraphNode * node4 = newGraphNode(graph,4);
    GraphNode * node5 = newGraphNode(graph,5);
    //node1-'h'->node2;
    char a = 'a';//-->Necesary?}
    addEdge(node1,node2,(void *)&a,CHAR_EDGE);
    addEdge(node1, node3,(void *)&a,CHAR_EDGE);
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
    
    //foreach(node in graph)

    printGraph(graph);

    // destroy node1
    removeNode(graph, node1);
    printGraph(graph);
    freeGraph(graph);
}
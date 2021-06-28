
// #---------------------------------------------#
// 	C-Graph compiler - 2021 - 1Q - TLAC
// 	A compiler to parse .cg files, a graph oriented programming language
//
//
// 	Created by:
// 		- Gaston Donikian
//		- Francisco Quesada
//		- Octavio Serpe
//		- Segundo Espina
// #---------------------------------------------#
#define MAX_INPUT_SIZE 256
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define START_SIZE 32
#define STR_MAX_LEN 64

typedef enum edgeType
{
    INT_EDGE,
    STRING_EDGE
} edgeType;

typedef struct GraphEdge
{
    edgeType type;
    void *weight;
} GraphEdge;

typedef struct GraphNode
{
    struct GraphNodeListElement *lastNode;
    struct GraphNodeListElement *firstNode;
    struct GraphEdgeListElement *lastEdge;
    struct GraphEdgeListElement *firstEdge;
    struct GraphEdgeListElement *it;
    size_t size;
    char * value;
    size_t id;
} GraphNode;

typedef struct Graph
{
    // GraphNode * root;
    struct GraphNodeListElement *last;
    struct GraphNodeListElement *first;
    struct GraphNodeListElement *it;
    size_t size;
} Graph;

typedef struct GraphNodeListElement
{
    GraphNode *value;
    struct GraphNodeListElement *next;
} GraphNodeListElement;

typedef struct GraphEdgeListElement
{
    GraphEdge *value;
    struct GraphEdgeListElement *next;
} GraphEdgeListElement;

void breakpoint()
{
    int *p = NULL;
    printf("%d\n", *p);
}

Graph *newGraph()
{
    Graph *g = calloc(1, sizeof(Graph));
    return g;
}

/* private */ void addGraphNode(Graph *graph, GraphNode *node)
{
    if (graph->size == 0)
    {
        graph->first = calloc(1, sizeof(GraphNodeListElement));
        graph->first->value = node;
        graph->first->next = NULL;
        graph->last = graph->first;
        graph->size++;
        return;
    }
    graph->last->next = calloc(1, sizeof(GraphNodeListElement));
    graph->last->next->value = node;
    graph->last->next->next = NULL;
    graph->last = graph->last->next;
    graph->size++;
}

GraphNode *newGraphNode(Graph *graph, char * value)
{
    GraphNode *n = calloc(1, sizeof(GraphNode));
    int len = strnlen(value, 256);
    n->value = malloc(sizeof(char) * (len+1));
    strncpy(n->value, value,len + 1);
    n->id = graph->size;
    addGraphNode(graph, n);
    return n;
}

/* private */ GraphEdge *newEdge(void *value, edgeType type)
{
    GraphEdge *e = calloc(1, sizeof(GraphEdge));
    int size = 0;
    switch (type)
    {
    case INT_EDGE:
        size = sizeof(int);
        break;
    case STRING_EDGE:
        size = sizeof(char) * strnlen((char *)value, STR_MAX_LEN);
        break;
    default:
        break;
    }
    e->type = type;
    e->weight = malloc(size);
    memcpy(e->weight, value, size);
    return e;
}

bool isAdjacent(GraphNode *origin, GraphNode *destination)
{

    for (GraphNodeListElement *n = origin->firstNode; n != NULL; n = n->next)
    {
        if (n->value == destination)
        {
            return true;
        }
    }
    return false;
}

void addEdge(GraphNode *node1, GraphNode *node2, void *value, edgeType type)
{
    if (node1->size == 0)
    {   
        node1->firstNode = calloc(1, sizeof(GraphNodeListElement));
        node1->lastNode = node1->firstNode;
        node1->firstEdge = calloc(1, sizeof(GraphEdgeListElement));
        node1->lastEdge = node1->firstEdge;
        node1->firstEdge->value = newEdge(value, type);
        node1->firstNode->next = NULL;
        node1->lastEdge = node1->firstEdge;
        node1->firstNode->value = node2;
        node1->firstNode->next = NULL;
        node1->lastNode = node1->firstNode;
        node1->size++;
        return;
    }
    if (isAdjacent(node1, node2))
    {
        return;
    }
    node1->lastEdge->next = calloc(1, sizeof(GraphEdgeListElement));
    node1->lastEdge->next->value = newEdge(value, type);
    node1->lastEdge->next->next = NULL;
    node1->lastEdge = node1->lastEdge->next;
    node1->lastNode->next = calloc(1, sizeof(GraphNodeListElement));
    node1->lastNode->next->value = node2;
    node1->lastNode->next->next = NULL;
    node1->lastNode = node1->lastNode->next;
    node1->size++;
}

void setRoot(GraphNode *node)
{
    // graph->root = node;
}

/* private */ void freeEdge(GraphEdge *edge)
{
    free(edge->weight);
    free(edge);
    // printf("at %s\n", __func__);
}

/* private */ void freeEdges(GraphNode *node)
{
    GraphEdgeListElement *e = node->firstEdge, *currE = NULL;
    GraphNodeListElement *n = node->firstNode, *currN = NULL;
    while (e != NULL || n != NULL)
    {
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
void removeEdge(GraphNode *node1, GraphNode *node2)
{
    GraphEdgeListElement *e = node1->firstEdge;
    GraphNodeListElement *n = node1->firstNode;
    GraphEdgeListElement *prevE = NULL;
    GraphNodeListElement *prevN = NULL;

    if (n != NULL && n->value == node2)
    {
        node1->firstEdge = e->next;
        node1->firstNode = n->next;
        freeEdge(e->value);
        free(e);
        free(n);
        // printf("at %s\n", __func__);
        node1->size--;
        return;
    }
    while (n != NULL && n->value != node2)
    {
        prevE = e;
        prevN = n;
        e = e->next;
        n = n->next;
    }
    if (n == NULL)
        return;
    prevN->next = n->next;
    prevE->next = e->next;
    freeEdge(e->value);
    free(n);
    free(e);
    // printf("at %s\n", __func__);
    node1->size--;
}
/*
    * Hay que recorrer todos los nodos del grafo menos el que quiero eliminar y por cada uno eliminar su adyacencia
*/
/* private */ void removeAdjacency(Graph *graph, GraphNode *node)
{
    GraphNodeListElement *n = graph->first;
    while (n != NULL)
    {
        if (n->value != node)
        {
            removeEdge(n->value, node);
        }
        n = n->next;
    }
}

// actualiza los indices de los nodos a partir del elemento recibido inclusive
/* private */ void updateIndexFromNode(GraphNodeListElement *element)
{
    while (element != NULL)
    {
        element->value->id--;
        element = element->next;
    }
}

void removeNode(Graph *graph, GraphNode *node)
{
    GraphNodeListElement *n = graph->first;
    GraphNodeListElement *prevN = NULL;
    if (n != NULL && n->value == node)
    {
        if (n == graph->last)
        {
            graph->last = prevN;
        }
        updateIndexFromNode(n->next);
        removeAdjacency(graph, n->value);
        graph->first = n->next;
        graph->size--;
        freeEdges(n->value);
        free(n->value);
        free(n);
        // printf("at %s\n", __func__);
        return;
    }

    while (n != NULL && n->value != node)
    {
        prevN = n;
        n = n->next;
    }
    if (n == NULL)
    {
        return;
    }
    if (n == graph->last)
    {
        graph->last = prevN;
    }
    updateIndexFromNode(n->next);
    removeAdjacency(graph, n->value);
    prevN->next = n->next;
    graph->size--;
    freeEdges(n->value);
    free(n->value);
    free(n);
    // printf("at %s\n", __func__);
}

void freeGraph(Graph *graph)
{
    GraphNodeListElement *it = graph->first, *next;

    while (it != NULL)
    {
        next = it->next;
        removeNode(graph, it->value);
        it = next;
    }
    free(graph);
    // printf("at %s\n", __func__);
}

void printfEdge(GraphEdge * edge){
    switch(edge->type){
        case STRING_EDGE:
            printf("%s",(char *) edge->weight);
        break;
        case INT_EDGE:
            printf("%d",*((int *)(edge->weight)));
        break;
    }
}

void printNodeList(GraphNode *node)
{
    GraphEdgeListElement *e = node->firstEdge;
    GraphNodeListElement *n = node->firstNode;
    while (e != NULL || n != NULL)
    {
        switch (e->value->type){
            case STRING_EDGE:
                printf("%s--%s->%s\n", node->value, ((char *)e->value->weight), n->value->value);
            break;
            case INT_EDGE:
                    printf("%s--%d->%s\n", node->value, *((int *)e->value->weight), n->value->value);
            break;
        }
        e = e->next;
        n = n->next;
    }
    printf("\n\n");
}

void printGraph(Graph *graph)
{
    GraphNodeListElement *n = graph->first;
    while (n != NULL)
    {
        printf("\nPrinting adjacency for node %s\n", n->value->value);
        printNodeList(n->value);
        n = n->next;
    }
}

void beginForEachNode(Graph *graph)
{
    graph->it = graph->first;
}

bool hasNextForEachNode(Graph *graph)
{
    return graph->it != NULL;
}

GraphNode *nextForEachNode(Graph *graph)
{
    if (!hasNextForEachNode(graph))
        return NULL;
    GraphNodeListElement *aux = graph->it;
    graph->it = graph->it->next;
    return aux->value;
}

void beginForEachEdge(GraphNode *node)
{
    node->it = node->firstEdge;
}

bool hasNextForEachEdge(GraphNode *node)
{
    return node->it != NULL;
}

GraphEdge *nextForEachEdge(GraphNode *node)
{
    if (!hasNextForEachEdge(node))
        return NULL;
    GraphEdgeListElement *aux = node->it;
    node->it = node->it->next;
    return aux->value;
}

void printNodesId(int *nodes, int idx)
{
    printf("\n------------------\n");
    for (int i = 0; i < idx; ++i)
    {
        printf("%d -> ", nodes[i]);
    }
    printf("\n------------------\n");
}

void traverseDFSRecursive(Graph *graph, GraphNode *node, bool *visited, int *nodes, int *idx, bool *printed)
{
    GraphNodeListElement *n = node->firstNode;
    if (visited[node->id] == false)
    {
        visited[node->id] = true;
        nodes[(*idx)++] = node->id;
    }
    while (n != NULL)
    {
        if (visited[n->value->id] == false)
        {
            if (*printed == true)
                *printed = false;
            visited[n->value->id] = true;
            nodes[(*idx)++] = n->value->id;
            traverseDFSRecursive(graph, n->value, visited, nodes, idx, printed);
            (*idx)--;
            visited[n->value->id] = false;
        }
        n = n->next;
    }
    if (*printed == false)
    {
        printNodesId(nodes, *idx);
        *printed = true;
    }
}

void traverseDFS(Graph *graph, GraphNode *node)
{
    printf("-----------    Traversing graph DFS for node ID %zu    -----------\n", node->id);
    bool visited[graph->size];
    for (int i = 0; i < graph->size; ++i)
        visited[i] = false;
    int nodes[graph->size];
    int idx = 0;
    bool printed = false;
    traverseDFSRecursive(graph, node, visited, nodes, &idx, &printed);
}

void traverseBFSRecursive(Graph *graph, GraphNode *node, bool *visited, GraphNode *nodes[], int *idx, int *start)
{
    GraphNodeListElement *n = node->firstNode;
    if (visited[node->id] == false)
    {
        visited[node->id] = true;
        nodes[(*idx)++] = node;
    }
    while (n != NULL)
    {
        if (visited[n->value->id] == false)
        {
            visited[n->value->id] = true;
            nodes[(*idx)++] = n->value;
        }
        n = n->next;
    }
    if (*start > 0)
        return;
    while (*start < *idx)
    {
        if (++(*start) == *idx)
            return;
        traverseBFSRecursive(graph, nodes[*start], visited, nodes, idx, start);
    }
}

void traverseBFS(Graph *graph, GraphNode *node)
{
    printf("-----------    Traversing graph BFS for node ID %zu    -----------\n", node->id);
    bool visited[graph->size];
    for (int i = 0; i < graph->size; ++i)
        visited[i] = false;
    GraphNode *nodes[graph->size];
    int nodesId[graph->size];
    int idx = 0, start = 0;
    traverseBFSRecursive(graph, node, visited, nodes, &idx, &start);
    for (int i = 0; i < idx; ++i)
    {
        nodesId[i] = nodes[i]->id;
    }
    printNodesId(nodesId, idx);
}
int main(){
Graph * graph = newGraph();
GraphNode * nodeIterator = NULL;
GraphEdge * edgeIterator = NULL;
printf("Hello World\n");
}
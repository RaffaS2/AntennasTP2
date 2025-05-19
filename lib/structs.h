/**
 * @file functions.h
 * @author a24869
 * @brief Ficheiro para chamar as structs 
 * 
 * @date 2025-05-08
 * 
 * 
 */

#ifndef structs_h
#define structs_h

typedef struct AdjList{
    struct Vertex *vertex; //Pointer que aponta para o vértice de destino da ligação
    struct AdjList *next; //Pointer para o próximo vértice daa lista ligada de arestas
} AdjList;

typedef struct Vertex{ 
    int x, y;
    char frequency;
    bool visited; // para o DFS e BFS, serve para evitar visitar o mesmo vértice mais do que uma vez
    struct AdjList *adjacents; //Pointer para o próximo vértice daa lista ligada de vértices
    struct Vertex *next; //Pointer que permite cirar  uma lista ligada de vaarias conexões (arestas neste caso)
} Vertex;

typedef struct Graph{
    Vertex *vertices; // Cabeça da lista dos vértices
} Graph;

#endif
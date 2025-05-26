/**
 * @file structs.h
 * @author a24869
 * @brief Definição das estruturas de dados utilizadas no grafo.
 * 
 * @date 2025-05-08
 * 
 * 
 */

#ifndef structs_h
#define structs_h

/**
 * @struct AdjList
 * @brief Representa uma aresta (ligação) entre vértices no grafo.
 * 
 * Cada nó da lista de adjacência representa uma ligação entre o vértice 
 * de origem e o vértice apontado pelo ponteiro `vertex`.
 */
typedef struct AdjList{
    struct Vertex *vertex; //Pointer que aponta para o vértice de destino da ligação
    struct AdjList *next; //Pointer para o próximo vértice daa lista ligada de arestas
} AdjList;

/**
 * @struct Vertex
 * @brief Representa um vértice (Antena) no grafo.
 * 
 * Contém informação posicional (x, y), uma frequência (carácter), 
 * um marcador de visitação para algoritmos de grafos, e ponteiros 
 * para listas de adjacência e para o próximo vértice.
 */
typedef struct Vertex{ 
    int x, y;
    char frequency;
    bool visited; // para o DFS e BFS, serve para evitar visitar o mesmo vértice mais do que uma vez
    struct AdjList *adjacents; //Pointer para o próximo vértice daa lista ligada de vértices
    struct Vertex *next; //Pointer que permite cirar  uma lista ligada de vaarias conexões (arestas neste caso)
} Vertex;

/**
 * @struct Graph
 * @brief Estrutura principal que representa o grafo.
 * 
 * Contém um pointer para o primeiro vértice da lista de vértices.
 */
typedef struct Graph{
    Vertex *vertices; // Cabeça da lista dos vértices
} Graph;

#endif
/**
 * @file functions.h
 * @author a24869
 * @brief Ficheiro para chamar as funções
 * 
 * @date 2025-05-08
 * 
 * 
 */
#ifndef functions_h
#define functions_h

#include "structs.h"
#include <stdbool.h>

Graph *createGraph();
Vertex *createVertex(int x, int y, char frequency);
void addVertex(Graph *g, int x, int y, char frequency);
void addEdge(Vertex *a, Vertex *b);
void printGraph(Graph *g);

Graph *readGraphFromFile(const char *filename);
void writeGraphToBinary(const char *filename, Graph *g);
Graph *readGraphFromBinary(const char *filename);

//DFS
void depthFirstSearch(Graph* g, int startX, int startY, char freq);
//BFS
void breadthFirstSearch(Graph* g, int startX, int startY, char freq);
//FAP
void findAllPaths(Graph* g, int x1, int y1, int x2, int y2, char freq);
//LIBF
void listIntersectionsBetweenFrequencies(Graph *g, char frequencyA, char frequencyB);


void freeGraph(Graph *g);

#endif
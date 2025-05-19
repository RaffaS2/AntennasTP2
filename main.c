/**
 * @file main.c
 * @author a24869
 * @brief Ficheiro principal para a gestão e manipulação de antenas.
 * 
 * Este programa lida com a inserção, remoção e manipulação de antenas, aplicando efeitos 
 * e armazenando os resultados num ficheiro. O código foi desenvolvido para gerir antenas
 * através de uma lista ligada e manipular as suas coordenadas numa matriz.
 * 
 * As funções principais do programa incluem:
 * - Inserir antenas em posições específicas.
 * - Remover antenas específicas.
 * - Guardar o estado das antenas e da matriz num ficheiro.
 * - Carregar as antenas de volta a partir de um ficheiro.
 * - Procura em profundidade a partir de uma determinada antena
 * - Procura em largura a partir de uma determinada antena
 * - Procura todos os caminhos entre duas antenas
 * - Liberta a memória de ambos os grafos criados
 * 
 * @date 2025-05-08
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lib/functions.h"

int main() {
    Graph *g = NULL;  // Declara e inicializa como NULL
    g = readGraphFromFile("data/antennasFile1.txt");  // Depois atribui o grafo lido

    if (g == NULL) {
        printf("Antenna matrix is empty after reading from text file\n");
    } else {
        printGraph(g);
        printf("\n");

        printf("dFS from vertex (1, 1) [B]:\n");
        depthFirstSearch(g, 1, 1, 'B');
        printf("\n");

        printf("bFS from vertex  (1, 1) [B]:\n");
        breadthFirstSearch(g, 1, 1, 'B');
        printf("\n");

        printf("Find all paths from (1, 1) [B] and (3, 7) [B]:\n");
        findAllPaths(g, 1, 1, 3, 7, 'B');
        printf("\n");
        
        printf("Intersections between frequency A and B:\n");
        listIntersectionsBetweenFrequencies(g, 'A', 'B');
        printf("\n");
        
        // Guardar o grafo em ficheiro binário
        writeGraphToBinary("data/antennas.bin", g);
            printf("Antennas graphs successfully saved to antennas.bin\n");
    
        // Ler novamente o grafo do ficheiro binário
        Graph *gFromBin = readGraphFromBinary("data/antennas.bin");
        if (gFromBin == NULL) {
            printf("Error reading Antennas Graphs from binary file\n");
        } else {
            printf("Antennas Graphs successfully read from binary file\n");
            printGraph(gFromBin);
            freeGraph(gFromBin);
        }  
    }   
    freeGraph(g); 

    return 0;
}


/*// Criar grafo manualmente
    Graph* g = createGraph();

    // Adicionar antenas manualmente
    addVertex(g, 5, 6, 'A');  
    addVertex(g, 7, 8, 'A');
    addVertex(g, 10, 4, 'B');
    addVertex(g, 12, 4, 'B');
    addVertex(g, 1, 1, 'C');  

    // Verificar ligação automática entre antenas com mesma frequência
    printf("Graph manually created\n");
    printGraph(g);
    */
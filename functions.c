/**
 * @file functions.h
 * @author a24869
 * @brief Ficheiro para implementar todas as funções chamadas no functions.h
 * 
 * @date 2025-05-08
 * 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "lib/functions.h"

/**
 * @brief Cria e inicializa um grafo vazio
 * 
 * Aloca a memória para a estrutura do graafo.
 * Inicializa a lista de vértices como NULL.
 * 
 * @return Graph* Pointer para o grafo criado
 */
Graph *createGraph(){
    Graph *g = (Graph*)malloc(sizeof(Graph));
    g->vertices = NULL;
    return g;
}
/**
 * @brief Cria um novo vértice (antena)
 * 
 * Esta função aloca aa memória para um novo vértice e define os seus atributos.
 * No fim inicializa as adjacencias e o próximo vértice como NULL.  
 * 
 * @param x Coordenada X
 * @param y Coordenada Y
 * @param frequency Carácter que representa a frequência da antena
 * @return Vertex* Pointer para o vértice criado
 */
Vertex *createVertex(int x, int y, char frequency){
    Vertex *v = (Vertex*)malloc(sizeof(Vertex));
    v->x = x;
    v->y = y;
    v->frequency = frequency;
    // indicicar que a nova antena não tem adjacentes
    v->adjacents = NULL;
    // indicar que ainda não é para estar ligado a outro vértice na lista do grafo
    v->next = NULL;

    return v;
}

/**
 * @brief Adiciona uma aresta bidirecional entre dois vértices
 * 
 * Esta função cria ligações entre dois vértices (antenas), adicionando cada uma na lista de adjacentes.
 * 
 * @param a Vértice A
 * @param b Vértice B
 */
void addEdge(Vertex *a, Vertex *b){
    // adiciona B à lista de vizinhos (adjacência) de A
    AdjList *adjB = (AdjList*)malloc(sizeof(AdjList));
    adjB->vertex = b;
    adjB->next = a->adjacents;
    a->adjacents = adjB;

    // adiciona A à lista de vizinhos (adjacência) de B
    AdjList *adjA = (AdjList*)malloc(sizeof(AdjList));
    adjA->vertex = a;
    adjA->next = b->adjacents;
    b->adjacents = adjA;
}

/** 
 * @brief Adiciona um novo vértice ao grafo e conecta com os da mesma frequência
 * 
 * Esta função adiciona o vértice àa lista de vértices do grafo e conecta com arestas 
 * os outros vértices da mesma frequencia
 * 
 * @param g Pointer para o grafo
 * @param x Coordenada X do novo vértice
 * @param y Coordenada Y do novo vértice
 * @param frequency Frequência do novo vértice
 */
void addVertex(Graph *g, int x, int y, char frequency){
    // Cria um novo vértice com as coordenadas e frequência indicadas
    Vertex *newV = createVertex(x, y, frequency);

    // Conectar com vértices da mesma frequência
    Vertex *current = g->vertices;;
    while (current != NULL){
        if(current->frequency == frequency){
            addEdge(newV, current);
        }
        current = current->next;
    }

    // Inserir novo vértice na lista de vértices
    newV->next = g->vertices;
    g->vertices = newV;
}

/**
 * @brief Imprime o conteúdo do grafo
 * 
 * Função para imprimir todos os vértices do grafo, assinalando as coordenadas
 * e frequencia, bem como os vértices adjacentes.
 * 
 * @param g Pointer para grafo
 */
void printGraph(Graph* g) {
    // Variável auxiliar para percorrer todos os vértices do grafo
    Vertex *v = g->vertices;
    while (v != NULL) {
            printf("Antenna (%d, %d) [%c] -> ", v->x, v->y, v->frequency);
        // Variável auxiliar para percorrer todas as adjaacentes dos vértices
        AdjList *adj = v->adjacents;
        while (adj != NULL) {
            printf("(%d, %d) ", adj->vertex->x, adj->vertex->y);
            adj = adj->next;
        }
        printf("\n");
        v = v->next;
    }
}

/**
 * @brief Lê um grafo de um ficheiro de texto
 * 
 * Esta função lê cada linha do ficheiro, interpreta caracteres diferentes de '.' como antenas,
 * e insere um vértice com a frequência correspondente e as respetivas coordenadas (linha, coluna).
 * Cada antena é adicionada ao grafo como um novo vértice.
 * 
 * @param filename Nome de ficheiro de texto a ser lido
 * @return Graph* Pointer para o grafo construído
 */
Graph *readGraphFromFile(const char *filename){
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        return NULL;
    }

    Graph *g = createGraph();
    char *line = NULL; // Pointer que vai guardar cada linha lida
    size_t len = 0;    // Tamanho do buffer (é ajustado automaticamente pelo getline)
    int x = 0;

  //lê cada linha até ao final do ficheiro
  while (getline(&line, &len, file) != -1) {
        int y = 0; // Agora y representa a coluna
        for (int i = 0; line[i] != '\0'; i++) {
            // Ignora espaços ou tabs
            if (line[i] == ' ' || line[i] == '\t') continue;

            // '.' representa célula vazia, ignora
            if (line[i] != '.' && line[i] != '\n') {
                // Troca x e y na chamada
                addVertex(g, x, y, line[i]);
            }
            y++;
        }
        x++;
    }

    free(line);
    fclose(file);
    return g;
}

/**
 * @brief Escreve um grafo para um ficheiro binário.
 * 
 * Esta função guarda todos os vértices e as suas adjacências num ficheiro binário,
 * no seguinte formato: número total de vértices, dados de cada vértice (coordenadas x, y e frequência),
 * seguido pelo número de adjacências e as coordenadas de cada vértice adjacente.
 * 
 * @param filename Nome do ficheiro binário do grafo que será escrito.
 * @param g Pointer para o grafo a ser escrito.
 */
void writeGraphToBinary(const char *filename, Graph *g) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;

    // Conta vértices
    int vertexCount = 0;
    Vertex *v = g->vertices;
    while (v) {
        vertexCount++;
        v = v->next;
    }
    
    //parametros da fwrite e fread (*ptr, size_t size, size_t count, file *stream)
    fwrite(&vertexCount, sizeof(int), 1, f);

    // Escreve vértices
    v = g->vertices;
    while (v) {
        fwrite(&v->x, sizeof(int), 1, f);
        fwrite(&v->y, sizeof(int), 1, f);
        fwrite(&v->frequency, sizeof(char), 1, f);
        v = v->next;
    }

    // Escreve adjacências (coordenadas dos vértices adjacentes)
    v = g->vertices;
    while (v) {
        // Conta adjacentes
        int adjCount = 0;
        AdjList *adj = v->adjacents;
        while (adj) {
            adjCount++;
            adj = adj->next;
        }
        fwrite(&adjCount, sizeof(int), 1, f);

        // Escrever adjacentes (coordenadas)
        adj = v->adjacents;
        while (adj) {
            int x = adj->vertex->x;
            int y = adj->vertex->y;
            fwrite(&x, sizeof(int), 1, f);
            fwrite(&y, sizeof(int), 1, f);
            adj = adj->next;
        }
        v = v->next;
    }

    fclose(f);
}

/**
 * @brief Lê um grafo a partir de um ficheiro binário.
 * 
 * Esta função reconstrói um grafo previamente guardado em formato binário, criando os vértices
 * e estabelecendo as adjacências com base nas coordenadas guardadas.
 * 
 * @param filename Nome do ficheiro binário de onde o grafo será lido.
 * @return Pointer para o grafo reconstruído. Retorna NULL em caso de erro na leitura.
 */
Graph *readGraphFromBinary(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) return NULL;

    Graph *g = createGraph();

    int vertexCount;
    fread(&vertexCount, sizeof(int), 1, f);

    // Criar todos os vértices e inserir no grafo
    for (int i = 0; i < vertexCount; i++) {
        int x, y;
        char freq;
        fread(&x, sizeof(int), 1, f);
        fread(&y, sizeof(int), 1, f);
        fread(&freq, sizeof(char), 1, f);
        addVertex(g, x, y, freq);  // adiciona já ao grafo
    }

    // Agora ler as adjacências e ligar os vértices
    for (int i = 0; i < vertexCount; i++) {
        int adjCount;
        fread(&adjCount, sizeof(int), 1, f);

        // Pega o vértice que está na posicao i da lista (simplesmente percorrer)
        Vertex *v = g->vertices;
        for (int j = 0; j < vertexCount - i - 1; j++) {
            v = v->next;
        }

        for (int k = 0; k < adjCount; k++) {
            int x_adj, y_adj;
            fread(&x_adj, sizeof(int), 1, f);
            fread(&y_adj, sizeof(int), 1, f);

            // Encontrar o vértice adjacente pela coordenada
            Vertex *adjV = g->vertices;
            while (adjV != NULL) {
                if (adjV->x == x_adj && adjV->y == y_adj) break;
                adjV = adjV->next;
            }
        }
    }

    fclose(f);
    return g;
}

/**
 * @brief Visita recursiva para Depth-First Search (DFS)
 * 
 * Esta função marca o vértice como visitado, imprime-o, e recursivamente visita os seus adjacentes
 * que tenham a mesma frequência e ainda não tenham sido visitados.
 * 
 * DFS em output: visita sempre o primeiro adjacente de cada vértice
 * 
 * @param v Pointer para o vértice atual
 */
void dfsVisit(Vertex* v) {
    if (v == NULL || v->visited)
        return;

    v->visited = true;
    printf("Visited: (%d, %d)\n", v->x, v->y);

    AdjList* adj = v->adjacents;
    while (adj != NULL) {
        Vertex* neighbor = adj->vertex;
        if (!neighbor->visited && neighbor->frequency == v->frequency) {
            dfsVisit(neighbor);
        }
        adj = adj->next;
    }
}

/**
 * @brief Função principal que inicia a Depth-First Search (DFS) no grafo
 * 
 * Procura o vértice inicial com as coordenadas e frequência dadas,
 * reseta o estado visited de todos os vértices,
 * e inicia a visita recursiva a partir do vértice inicial.
 *
 * 
 * @param g Pointer para o grafo
 * @param startX Coordenada x do vértice inicial
 * @param startY Coordenada y do vértice inicial
 * @param freq Frequência do vértice inicial
 */
void depthFirstSearch(Graph* g, int startX, int startY, char frequency) {
    Vertex* start = g->vertices;

    // Encontrar o vértice inicial
    while (start != NULL) {
        if (start->x == startX && start->y == startY && start->frequency == frequency)
            break;
        start = start->next;
    }

    if (start == NULL) {
        return;
    }

    // Reset aos visitados
    Vertex* v = g->vertices;
    while (v != NULL) {
        v->visited = false;  // Reset ao campo visited
        v = v->next;
    }

    dfsVisit(start);
}

/**
 * @brief Implementa a Breadth-First Search (BFS) no grafo
 * 
 * Procura o vértice inicial com as coordenadas e frequência dadas, reseta o estado visited de todos os vértices,
 * e usa uma estrutura em fila para visitar todos os vértices alcançáveis ligados a partir do vértice inicial, com a mesma frequência.
 * 
 * BFS em output: visita todos os adjacentes diretos primeiro
 * 
 * @param g Pointer para o grafo
 * @param startX Coordenada x do vértice inicial
 * @param startY Coordenada y do vértice inicial
 * @param freq Frequência do vértice inicial
 */
void breadthFirstSearch(Graph* g, int startX, int startY, char frequency) {
    Vertex* start = g->vertices;

    // Encontrar o vértice inicial
    while (start != NULL) {
        if (start->x == startX && start->y == startY && start->frequency == frequency)
            break;
        start = start->next;
    }

    if (start == NULL) {
        return;
    }

    // Resetar todos os vértices para "não visitados" 
    Vertex* v = g->vertices;
    while (v != NULL) {
        v->visited = false;
        v = v->next;
    }

    // Estrutura em fila para BFS (array de Pointers para vértices)
    Vertex* queue[100];
    int front = 0, back = 0;

    // Iniciar com o vértice inicial
    queue[back++] = start;
    start->visited = true;

    while (front < back) {
        Vertex* current = queue[front++];
        printf("Visited: (%d, %d)\n", current->x, current->y);

        AdjList* adj = current->adjacents;
        while (adj != NULL) {
            Vertex* neighbor = adj->vertex;
            if (!neighbor->visited && neighbor->frequency == current->frequency) {
                neighbor->visited = true;
                queue[back++] = neighbor;
            }
            adj = adj->next;
        }
    }
}

/**
 * @brief Função recursiva para encontrar todos os caminhos entre dois vértices com a mesma frequência
 *
 * Esta função explora recursivamente os vértices adjacentes do vértice atual, 
 * construindo um caminho temporário e imprimindo-o sempre que o vértice destino for alcançado.
 *
 * @param current Vértice atual na pesquisa
 * @param target Vértice destino que se pretende alcançar
 * @param path Array de pointers para os vértices que compõem o caminho atual
 * @param length Length controla o tamanho do caminho atual (ou seja, quantos vértices estão em path[])
 */
void findPathsRecursive(Vertex* current, Vertex* target, Vertex* path[], int length) {
    if (current == NULL || current->visited)
        return;

    // Marca o vértice atual como visitado para evitar ciclos
    current->visited = true;
    // Adiciona o vértice atual ao caminho na posição length e incrementa o array
    path[length] = current;
    length++;

    // Se chegou ao destino imprime o caminho
    if (current == target) {
        printf("Path Founded: ");
        for (int i = 0; i < length; i++) {
            printf("(%d,%d)", path[i]->x, path[i]->y);
            if (i < length - 1) printf(" -> ");
        }
        printf("\n");
    // Se não chegou ao destino, explora os vértices adjacentes válidos para continuar a procura
    } else {
        AdjList* adj = current->adjacents;
        while (adj != NULL) {
            Vertex* neighbor = adj->vertex;
            // Só visita o adjacente se ainda não foi visitado e tiver a mesma frequência que o atual
            if (!neighbor->visited && neighbor->frequency == current->frequency) {
                findPathsRecursive(neighbor, target, path, length);
            }
            adj = adj->next;
        }
    }

    current->visited = false; 
}

/**
 * @brief Encontra e imprime todos os caminhos entre dois vértices com frequência comum.
 *
 * Procura os vértices de origem e destino no grafo com as coordenadas e frequência indicadas.
 * Se ambos forem encontrados, inicia a pesquisa para listar todos os caminhos possíveis entre eles.
 *
 * @param g Pointer para o grafo
 * @param x1 Coordenada X do vértice de origem
 * @param y1 Coordenada Y do vértice de origem
 * @param x2 Coordenada X do vértice de destino
 * @param y2 Coordenada Y do vértice de destino
 * @param frequency Frequência comum requerida para os vértices do caminho
 */
void findAllPaths(Graph* g, int x1, int y1, int x2, int y2, char frequency) {
    Vertex* origin = NULL;
    Vertex* destination = NULL;
    Vertex* v = g->vertices;

    // Localizar os vértices de origem e destino
    while (v != NULL) {
        if (v->x == x1 && v->y == y1 && v->frequency == frequency)
            origin = v;
        if (v->x == x2 && v->y == y2 && v->frequency == frequency)
            destination = v;
        v->visited = false; // resetar visited
        v = v->next;
    }

    if (!origin || !destination) {
        return;
    }

    // Array temporário para guardar o caminho atual (tamanho máximo: nº de vértices)
    Vertex* path[100];
    findPathsRecursive(origin, destination, path, 0);
}

/**
 * @brief Verifica e lista de pares de vértices com frequência A e B que são vizinhos, considerando todas as direções adjacentes (4 sentidos diretos e 4 sentidos diagonais).
 * 
 * A função percorre todos os vértices do grafo que possuam vizinhos com uma frequencia distinta, 
 * nos 8 sentidos possíveis da função.
 * 
 * @param g Pointer para o grafo
 * @param frequencyA Frequências dos vértices A
 * @param frequencyB Frequências dos vértices A
 */
void listIntersectionsBetweenFrequencies(Graph *g, char frequencyA, char frequencyB) {
    Vertex *vA = g->vertices;
    while (vA != NULL) {
        if (vA->frequency == frequencyA) {
            Vertex *vB = g->vertices;
            while (vB != NULL) {
                if (vB->frequency == frequencyB) {
                    // Vizinhos sentido direto
                    int topNeighbor = (vA->x == vB->x && vA->y == vB->y + 1);
                    int bottomNeighbor = (vA->x == vB->x && vA->y == vB->y - 1);
                    int leftNeighbor = (vA->x == vB->x + 1 && vA->y == vB->y);
                    int rightNeighbor = (vA->x == vB->x - 1 && vA->y == vB->y);

                    // Vizinhos sentido diagonal
                    int topLeftNeighbor = (vA->x == vB->x + 1 && vA->y == vB->y + 1);
                    int topRightNeighbor = (vA->x == vB->x - 1 && vA->y == vB->y + 1);
                    int bottomLeftNeighbor = (vA->x == vB->x + 1 && vA->y == vB->y - 1);
                    int bottomRightNeighbor = (vA->x == vB->x - 1 && vA->y == vB->y - 1);

                    if (topNeighbor || bottomNeighbor || leftNeighbor || rightNeighbor ||
                        topLeftNeighbor || topRightNeighbor || bottomLeftNeighbor || bottomRightNeighbor) {
                        printf("Intersection between (%d, %d) [%c] and (%d, %d) [%c]\n",
                               vA->x, vA->y, vA->frequency,
                               vB->x, vB->y, vB->frequency);
                    }
                }
                vB = vB->next;
            }
        }
        vA = vA->next;
    }
}

/**
 * @brief Liberta toda a memória associada a um grafo
 * 
 * @param g Pointer para o grafo
 */
void freeGraph(Graph *g){
    if (g == NULL) return;

    Vertex *v = g->vertices;
    while (v != NULL) {
        // Libertar a lista de adjacentes
        AdjList *adj = v->adjacents;

        while (adj != NULL) {
            AdjList *tempAdj = adj;
            adj = adj->next;
            free(tempAdj);
        }
        // Libertar o vértice
        Vertex *tempV = v;
        v = v->next;
        free(tempV);
    }
    free(g);
}
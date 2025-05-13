#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Representa uma aresta em um grafo direcionado, com peso
typedef struct {
    int destino;   // Nó de destino
    int custo;     // Peso ou custo da aresta
} Conexao;

// Representa o grafo, usando lista de adjacência
typedef struct {
    Conexao** vizinhos; // Lista de adjacência (matriz de ponteiros)
    int* graus;         // Número de conexões para cada nó
    int totalVertices;  // Total de nós no grafo
} Graph;

// Estrutura usada na fila de prioridade (heap), contendo um nó e sua distância acumulada
typedef struct {
    int distancia;  // Distância acumulada a partir do nó origem
    int indice;     // Índice do nó atual
} EntradaHeap;

// Representa a heap mínima, usada no Dijkstra
typedef struct {
    EntradaHeap* elementos; // Vetor de elementos da heap
    int quantidade;         // Número atual de elementos na heap
    int capacidade;         // Capacidade máxima atual da heap
} MinHeap;

// Inicializa a heap com capacidade inicial
MinHeap* inicializarHeap(int capacidadeInicial) {
    MinHeap* novaHeap = (MinHeap*)malloc(sizeof(MinHeap));
    novaHeap->elementos = (EntradaHeap*)malloc(capacidadeInicial * sizeof(EntradaHeap));
    novaHeap->quantidade = 0;
    novaHeap->capacidade = capacidadeInicial;
    return novaHeap;
}

// Troca duas entradas da heap de lugar (função utilitária)
void trocar(EntradaHeap* a, EntradaHeap* b) {
    EntradaHeap temp = *a;
    *a = *b;
    *b = temp;
}

// Reorganiza a heap de baixo pra cima após inserção
void subirHeap(MinHeap* heap, int posicao) {
    while (posicao > 0) {
        int pai = (posicao - 1) / 2;
        if (heap->elementos[posicao].distancia >= heap->elementos[pai].distancia) break;
        trocar(&heap->elementos[posicao], &heap->elementos[pai]);
        posicao = pai;
    }
}

// Reorganiza a heap de cima pra baixo após remoção
void descerHeap(MinHeap* heap, int posicao) {
    while (2 * posicao + 1 < heap->quantidade) {
        int esquerda = 2 * posicao + 1;
        int direita = 2 * posicao + 2;
        int menor = esquerda;
        if (direita < heap->quantidade && heap->elementos[direita].distancia < heap->elementos[esquerda].distancia) {
            menor = direita;
        }
        if (heap->elementos[posicao].distancia <= heap->elementos[menor].distancia) break;
        trocar(&heap->elementos[posicao], &heap->elementos[menor]);
        posicao = menor;
    }
}

// Dobra a capacidade da heap quando necessário
void expandirHeap(MinHeap* heap) {
    heap->capacidade *= 2;
    heap->elementos = (EntradaHeap*)realloc(heap->elementos, heap->capacidade * sizeof(EntradaHeap));
}

// Insere um novo elemento na heap
void inserirHeap(MinHeap* heap, EntradaHeap item) {
    if (heap->quantidade == heap->capacidade) {
        expandirHeap(heap);
    }
    heap->elementos[heap->quantidade] = item;
    subirHeap(heap, heap->quantidade);
    heap->quantidade++;
}

// Remove e retorna o menor elemento da heap
EntradaHeap removerMinimo(MinHeap* heap) {
    EntradaHeap menor = heap->elementos[0];
    heap->elementos[0] = heap->elementos[heap->quantidade - 1];
    heap->quantidade--;
    descerHeap(heap, 0);
    return menor;
}

// Cria o grafo com os vértices e as arestas fornecidas
Graph* graphCreate(int numeroNos, int** arestas, int quantidadeArestas, int* colunasAresta) {
    Graph* novoGrafo = (Graph*)malloc(sizeof(Graph));
    novoGrafo->totalVertices = numeroNos;
    novoGrafo->vizinhos = (Conexao**)malloc(numeroNos * sizeof(Conexao*));
    novoGrafo->graus = (int*)calloc(numeroNos, sizeof(int));

    // Inicializa listas de vizinhos como NULL
    for (int i = 0; i < numeroNos; i++) {
        novoGrafo->vizinhos[i] = NULL;
    }

    // Adiciona todas as arestas fornecidas ao grafo
    for (int i = 0; i < quantidadeArestas; i++) {
        int origem = arestas[i][0];
        int destino = arestas[i][1];
        int peso = arestas[i][2];

        novoGrafo->vizinhos[origem] = (Conexao*)realloc(novoGrafo->vizinhos[origem],
                                         (novoGrafo->graus[origem] + 1) * sizeof(Conexao));
        novoGrafo->vizinhos[origem][novoGrafo->graus[origem]++] = (Conexao){destino, peso};
    }

    return novoGrafo;
}

// Adiciona uma nova aresta ao grafo já existente
void graphAddEdge(Graph* grafo, int* novaAresta, int tamanho) {
    int origem = novaAresta[0];
    int destino = novaAresta[1];
    int custo = novaAresta[2];

    grafo->vizinhos[origem] = (Conexao*)realloc(grafo->vizinhos[origem],
                                  (grafo->graus[origem] + 1) * sizeof(Conexao));
    grafo->vizinhos[origem][grafo->graus[origem]++] = (Conexao){destino, custo};
}

// Calcula o menor caminho entre dois nós usando Dijkstra
int graphShortestPath(Graph* grafo, int origem, int destino) {
    int* distancias = (int*)malloc(grafo->totalVertices * sizeof(int));

    // Inicializa todas as distâncias com infinito
    for (int i = 0; i < grafo->totalVertices; i++) {
        distancias[i] = INT_MAX;
    }
    distancias[origem] = 0;

    // Cria a heap de prioridades
    MinHeap* fila = inicializarHeap(grafo->totalVertices * 2);
    inserirHeap(fila, (EntradaHeap){0, origem});

    while (fila->quantidade > 0) {
        EntradaHeap atual = removerMinimo(fila);
        int noAtual = atual.indice;
        int distAtual = atual.distancia;

        if (distAtual > distancias[noAtual]) continue;

        for (int i = 0; i < grafo->graus[noAtual]; i++) {
            Conexao vizinho = grafo->vizinhos[noAtual][i];
            int proximoNo = vizinho.destino;
            int custoAresta = vizinho.custo;

            if (distancias[noAtual] + custoAresta < distancias[proximoNo]) {
                distancias[proximoNo] = distancias[noAtual] + custoAresta;
                inserirHeap(fila, (EntradaHeap){distancias[proximoNo], proximoNo});
            }
        }
    }

    int resultado = distancias[destino] == INT_MAX ? -1 : distancias[destino];

    free(distancias);
    free(fila->elementos);
    free(fila);

    return resultado;
}

// Libera toda a memória usada pelo grafo
void graphFree(Graph* grafo) {
    for (int i = 0; i < grafo->totalVertices; i++) {
        free(grafo->vizinhos[i]);
    }
    free(grafo->vizinhos);
    free(grafo->graus);
    free(grafo);
}

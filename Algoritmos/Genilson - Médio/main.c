#include <stdio.h>  
#include <stdlib.h>
#include <limits.h>

#define MAX_NOS 105
#define MAX_ARESTAS 6005
#define INFINITO 1000000000

// Estrutura de aresta
typedef struct Aresta {
    int destino, peso;
    struct Aresta* proxima;
} Aresta;

// Grafo como lista de adjacência
Aresta* grafo[MAX_NOS];
int visitado[MAX_NOS];
int distancia[MAX_NOS];

// Nó da fila de prioridade
typedef struct {
    int no, dist;
} NoFila;

NoFila heap[MAX_ARESTAS];
int tamanhoHeap = 0;

// Declaração da função principal
int networkDelayTime(int** tempos, int tamTempos, int* tamColuna, int qtdNos, int origem);

void adicionarAresta(int origem, int destino, int peso) {
    Aresta* nova = (Aresta*)malloc(sizeof(Aresta));
    nova->destino = destino;
    nova->peso = peso;
    nova->proxima = grafo[origem];
    grafo[origem] = nova;
}

void trocar(NoFila* a, NoFila* b) {
    NoFila temp = *a;
    *a = *b;
    *b = temp;
}

void inserirHeap(int no, int dist) {
    int i = tamanhoHeap++;
    heap[i].no = no;
    heap[i].dist = dist;

    while (i > 0) {
        int pai = (i - 1) / 2;
        if (heap[pai].dist <= heap[i].dist) break;
        trocar(&heap[pai], &heap[i]);
        i = pai;
    }
}

NoFila removerHeap() {
    NoFila topo = heap[0];
    heap[0] = heap[--tamanhoHeap];

    int i = 0;
    while (1) {
        int menor = i, esq = 2 * i + 1, dir = 2 * i + 2;
        if (esq < tamanhoHeap && heap[esq].dist < heap[menor].dist) menor = esq;
        if (dir < tamanhoHeap && heap[dir].dist < heap[menor].dist) menor = dir;
        if (menor == i) break;
        trocar(&heap[i], &heap[menor]);
        i = menor;
    }

    return topo;
}

int networkDelayTime(int** tempos, int tamTempos, int* tamColuna, int qtdNos, int origem) {
    // Reinicialização
    for (int i = 0; i <= qtdNos; i++) {
        grafo[i] = NULL;
        visitado[i] = 0;
        distancia[i] = INFINITO;
    }
    tamanhoHeap = 0;

    // Construir o grafo
    for (int i = 0; i < tamTempos; i++) {
        int u = tempos[i][0];
        int v = tempos[i][1];
        int w = tempos[i][2];
        adicionarAresta(u, v, w);
    }

    distancia[origem] = 0;
    inserirHeap(origem, 0);

    while (tamanhoHeap > 0) {
        NoFila atual = removerHeap();
        int u = atual.no;
        int d = atual.dist;

        if (visitado[u]) continue;
        visitado[u] = 1;

        for (Aresta* a = grafo[u]; a; a = a->proxima) {
            int v = a->destino;
            int w = a->peso;
            if (!visitado[v] && d + w < distancia[v]) {
                distancia[v] = d + w;
                inserirHeap(v, distancia[v]);
            }
        }
    }

    int atrasoMaximo = 0;
    for (int i = 1; i <= qtdNos; i++) {
        if (distancia[i] == INFINITO) return -1;
        if (distancia[i] > atrasoMaximo) atrasoMaximo = distancia[i];
    }

    return atrasoMaximo;
}

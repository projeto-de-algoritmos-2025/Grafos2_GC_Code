#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/*
 * Calcular a distância retilínea entre duas coordenadas no plano cartesiano.
 * A distância de Manhattan é a soma das diferenças das coordenadas x e y.
 */

int calcularDistanciaRetilinha(int* coordenadaA, int* coordenadaB) {
    return abs(coordenadaA[0] - coordenadaB[0]) + abs(coordenadaA[1] - coordenadaB[1]);
}

/*
 * Implementa o algoritmo de Prim para encontrar a árvore geradora mínima
 * em um grafo completo onde os vértices são pontos e as arestas são
 * as distâncias de Manhattan entre eles.
 */

int minCostConnectPoints(int** pontos, int pontosTamanho, int* pontosColunaTamanho) {

    // Array para rastrear quais pontos já foram incluídos na solução
    bool* pontosIncluidos = (bool*)calloc(pontosTamanho, sizeof(bool));

    // Array para armazenar os menores custos conhecidos para conectar cada ponto
    int* custosMinimos = (int*)malloc(pontosTamanho * sizeof(int));

    // Inicializa todos os custos com o valor máximo possível
    for (int i = 0; i < pontosTamanho; i++) {
        custosMinimos[i] = INT_MAX;
    }

    int custoTotal = 0;
    custosMinimos[0] = 0; // Começamos pelo primeiro ponto (custo zero para incluí-lo)

    // Processaremos todos os pontos um por um
    for (int iteracao = 0; iteracao < pontosTamanho; iteracao++) {
        int pontoAtual = -1;
        int menorCustoAtual = INT_MAX;

        // Encontra o ponto não incluído com o menor custo de conexão
        for (int j = 0; j < pontosTamanho; j++) {
            if (!pontosIncluidos[j] && custosMinimos[j] < menorCustoAtual) {
                menorCustoAtual = custosMinimos[j];
                pontoAtual = j;
            }
        }

        // Marca o ponto selecionado como incluído na solução
        pontosIncluidos[pontoAtual] = true;
        custoTotal += menorCustoAtual;

        // Atualiza os custos mínimos para os pontos vizinhos não incluídos
        for (int j = 0; j < pontosTamanho; j++) {
            if (!pontosIncluidos[j]) {

                // Calcula a distância entre o ponto atual e este vizinho
                int distancia = calcularDistanciaRetilinha(pontos[pontoAtual], pontos[j]);

                // Se encontramos uma conexão mais barata para este ponto, atualizamos
                if (distancia < custosMinimos[j]) {
                    custosMinimos[j] = distancia;
                }
            }
        }
    }

    // Libera a memória alocada para as estruturas auxiliares

    free(pontosIncluidos);
    free(custosMinimos);

    return custoTotal;
}
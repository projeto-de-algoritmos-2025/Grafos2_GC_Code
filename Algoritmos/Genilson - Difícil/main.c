// Você recebe dois grupos de pontos, onde o primeiro grupo tem pontos de tamanho 1, o segundo grupo tem pontos de tamanho 2.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX1 13
#define MAX2 13
#define INF 1000000000

// Função auxiliar para retornar o menor de dois valores
int minimo(int a, int b) {
    return a < b ? a : b;
}

// Tabelas
int memo[MAX1][1 << MAX2];
int visitado[MAX1][1 << MAX2];

int tam_grupo1, tam_grupo2;
int custo[MAX1][MAX2];
int menor_custo_grupo2[MAX2];

// Função recursiva para encontrar custo mínimo
int resolver(int indice, int mascara) {
    if (indice == tam_grupo1) {
        int custo_extra = 0;

        for (int j = 0; j < tam_grupo2; j++) {
            if ((mascara & (1 << j)) == 0) {
                custo_extra += menor_custo_grupo2[j];
            }
        }
        return custo_extra;
    }

    if (visitado[indice][mascara]) return memo[indice][mascara];

    int resultado = INF;

    // Tenta conectar o ponto atual com cada ponto do segundo grupo
    for (int j = 0; j < tam_grupo2; j++) {
        int nova_mascara = mascara | (1 << j);
        resultado = minimo(resultado, resolver(indice + 1, nova_mascara) + custo[indice][j]);
    }

    visitado[indice][mascara] = 1;
    memo[indice][mascara] = resultado;
    return resultado;
}

int connectTwoGroups(int** custo_entrada, int tamanho, int* colunas) {
    tam_grupo1 = tamanho;
    tam_grupo2 = colunas[0];

    for (int i = 0; i < tam_grupo1; i++)
        for (int j = 0; j < tam_grupo2; j++)
            custo[i][j] = custo_entrada[i][j];

    for (int j = 0; j < tam_grupo2; j++) {
        menor_custo_grupo2[j] = INF;
        for (int i = 0; i < tam_grupo1; i++) {
            menor_custo_grupo2[j] = minimo(menor_custo_grupo2[j], custo[i][j]);
        }
    }

    // Zera a tabela de visitados
    memset(visitado, 0, sizeof(visitado));

    return resolver(0, 0);
}
/**
 * @file inicializa.c
 * @author Alexandre Meslin
 * @version 1.0
 * @date 2026-08-27
 * Inicializa uma matriz de tamanho TAM x TAM 
 * e percorre a matriz de três formas diferentes, 
 * medindo o tempo de execução de cada uma.

 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM 20000

typedef struct s_matriz {
    int n_linhas;
    int n_colunas;
    float *p_matriz;
} t_matriz;

#define valor(matriz, linha, coluna) (*((matriz).p_matriz + (linha) * (matriz).n_colunas + (coluna)))

int main(void) {
    // time
    clock_t start;
    clock_t stop;
    t_matriz a;
    float *p;

    a.p_matriz = (float *)aligned_alloc(4*8, TAM * TAM * sizeof(float));
    if(!a.p_matriz) {
        fprintf(stderr, "[ERROR %d] Erro ao alocar memória para a matriz A\n", __LINE__);
        exit(1);
    }
    a.n_colunas = TAM;
    a.n_linhas = TAM;

    printf("Percorrendo a matriz por coluna\n");
    start = clock();
    for(int i=0; i<a.n_colunas; i++) {
        for(int j=0; j<a.n_linhas; j++) {
            valor(a, i, j) = (float)(i + j);
        }
    }
    stop = clock();
    printf("Tempo de execução: %f segundos\n", (double)(stop - start) / CLOCKS_PER_SEC);

    printf("Percorrendo a matriz por linha\n");
    start = clock();
    for(int i=0; i<a.n_linhas; i++) {
        for(int j=0; j<a.n_colunas; j++) {
            valor(a, i, j) = (float)(i + j);
        }
    }
    stop = clock();
    printf("Tempo de execução: %f segundos\n", (double)(stop - start) / CLOCKS_PER_SEC);

    printf("Percorrendo a matriz com ponteiro\n");
    start = clock();
    p = a.p_matriz;
    for(int i=0; i<a.n_linhas; i++) {
        for(int j=0; j<a.n_colunas; j++) {
            *p++ = (float)(i + j);
        }
    }
    stop = clock();
    printf("Tempo de execução: %f segundos\n", (double)(stop - start) / CLOCKS_PER_SEC);

    return 0;
}
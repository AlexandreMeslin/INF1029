#include <stdio.h>
#include <immintrin.h>
#include <string.h>
#include <time.h>

#include "comum.h"

// Constantes
#define QTD (10000000 * 8)

// Protótipos
void exibeResultado(float *result);
void subSemAVX(float *result, float *a, float *b);
void subComAVX(float *result, float *a, float *b);

int main(void) {
    float *a;
    float *b;
    float *result;
    clock_t start_time, stop_time;

    // cria os vetores a, b e result
    a = (float*) malloc(QTD * sizeof(float));
    if(a == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o vetor A\n");
        return 1;
    }
    fprintf(stderr, "[DEBUG %d] Vetor A alocado com sucesso\n", __LINE__);
    b = (float*) malloc(QTD * sizeof(float));
    if(b == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o vetor B\n");
        free(a);
        return 1;
    }
    fprintf(stderr, "[DEBUG %d] Vetor B alocado com sucesso\n", __LINE__);
    result = (float*) malloc(QTD * sizeof(float));
    if(result == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o vetor Result\n");
        free(a);
        free(b);
        return 1;
    }
    fprintf(stderr, "[DEBUG %d] Vetor Result alocado com sucesso\n", __LINE__);

    // inicializa os vetores
    for(int i=0; i<QTD; i++) {
        a[i] = 2*i;
        b[i] = i;
    }
    fprintf(stderr, "[DEBUG %d] Vetores A e B inicializados com sucesso\n", __LINE__);

    start_time = clock();
    subSemAVX(result, a, b);
    stop_time = clock();
    printf("\nTempo gasto sem AVX: %f ms\n", timedifference_msec(start_time, stop_time));
    exibeResultado(result);

    start_time = clock();
    subComAVX(result, a, b);
    stop_time = clock();
    printf("\nTempo gasto com AVX: %f ms\n", timedifference_msec(start_time, stop_time));
    exibeResultado(result);

    // libera a memória alocada
    free(a);
    free(b);
    free(result);

    return 0;
}

void exibeResultado(float *result) {
    for(int i=0; i<64; i++) {
        if(!(i%8)) putchar('\n');
        printf("%6.2f ", result[i]);
    }
    putchar('\n');
}

void subSemAVX(float *result, float *a, float *b) {
    for(int i=0; i<QTD; i++) {
        result[i] = a[i] - b[i];
    }
}

void subComAVX(float *result, float *a, float *b) {
    for(int i=0; i<QTD; i+=8) {
        __m256 vetor_a;
        __m256 vetor_b;
        __m256 vetor_result;

        vetor_a = _mm256_set_ps (a[7+i], a[6+i], a[5+i], a[4+i], a[3+i], a[2+i], a[1+i], a[0+i]);
        vetor_b = _mm256_set_ps (b[7+i], b[6+i], b[5+i], b[4+i], b[3+i], b[2+i], b[1+i], b[0+i]);
        vetor_result = _mm256_sub_ps (vetor_a, vetor_b);
        memcpy(result+i, &vetor_result, 8*4);
    }
}

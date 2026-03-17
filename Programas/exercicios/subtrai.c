#include <stdio.h>
#include <immintrin.h>
#include <string.h>

#define QTD (1000000 * 8)

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

int main(void) {
    float a[QTD];
    float b[QTD];
    float result[QTD];

    // inicializa os vetores
    for(int i=0; i<QTD; i++) {
        a[i] = 2*i;
        b[i] = i;
    }

    subSemAVX(result, a, b);
    exibeResultado(result);

    subComAVX(result, a, b);
    exibeResultado(result);

    return 0;
}
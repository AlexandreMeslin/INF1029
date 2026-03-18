#include <stdio.h>
#include <stdlib.h>
#include "comum.h"

void processaVetores(data_t *hmA, data_t *hvB, int nIncognitas) {
    // Transforma a matriz A e o vetor B
    for(int passo=1; passo<nIncognitas; passo++) {
        for(int linha=passo; linha<nIncognitas; linha++) {
            data_t multiplicador = matriz(hmA, linha, passo-1, nIncognitas) / matriz(hmA, passo-1, passo-1, nIncognitas);
            for(int coluna=passo-1; coluna<nIncognitas; coluna++) {
                matriz(hmA, linha, coluna, nIncognitas) -= matriz(hmA, passo-1, coluna, nIncognitas) * multiplicador;
            }
            hvB[linha] -= hvB[passo-1] * multiplicador;
        }
    }
}
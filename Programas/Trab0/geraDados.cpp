/*
Gera a massa de dados
Uso: geraDados <número de incógnitas>
Serão gerados os arquivos Matriz_A.bin e Vetor_B.bin no diretório corrente
*/
#include <stdio.h>
#include <stdlib.h>
#include "comum.h"

#undef ARG_N_INCOGNITAS
#undef ARGC
#define ARG_N_INCOGNITAS    1
#define ARGC                2

#define MAXIMO              10

/**
 * Gera a massa de dados para o trabalho. 
 * O número de incógnitas é passado como argumento. 
 * Os arquivos Matriz_A.bin e Vetor_B.bin são gerados no diretório corrente. 
 * Cada elemento é um número aleatório entre 0 e MAXIMO. 
 * O formato dos arquivos é binário, ou seja, cada elemento é escrito usando fwrite. 
 * O número de elementos em Matriz_A.bin é nIncognitas*nIncognitas e o número de elementos em Vetor_B.bin é nIncognitas.
 * 
 * Para compilar:
 * $ gcc -Wextra -Wall -o geraDados geraDados.cpp
 * 
 * Exemplo de uso:
 * $ ./geraDados 5
 * Isso irá gerar os arquivos Matriz_A.bin e Vetor_B.bin com 25 e 5 elementos, respectivamente, para o caso de 5 incógnitas.
 */
int main(int argc, char *argv[]) {
    FILE *arq;          /**< Ponteiro para o arquivo de saída (matriz A e vetor B) */
    int nIncognitas;    /**< Número de incógnitas */
    data_t valor;       /**< Variável para armazenar o valor aleatório gerado */

    // Verifica se o número de argumentos é correto
    if(argc != ARGC) {
        fprintf(stderr, "Uso: %s <número de incógnitas>\n", argv[0]);
        exit(1);
    }

    // Converte o argumento para inteiro e armazena em nIncognitas
    nIncognitas = atoi(argv[ARG_N_INCOGNITAS]);

    // Gera a matriz A e o vetor B com valores aleatórios entre 0 e MAXIMO
    // Escreve a matriz A no arquivo Matriz_A.bin
    if((arq = fopen("Matriz_A.bin", "wb")) == NULL) {
        fprintf(stderr, "Não foi possível abrir o arquivo Matriz_A.bin para escrita\n");
        exit(2);
    }
    for(int i=0; i<nIncognitas*nIncognitas; i++) {
        valor = (data_t)rand()/(data_t)(RAND_MAX/MAXIMO);
        fwrite(&valor, sizeof valor, 1, arq);
    }
    fclose(arq);

    // Escreve o vetor B no arquivo Vetor_B.bin
    if((arq = fopen("Vetor_B.bin", "wb")) == NULL) {
        fprintf(stderr, "Não foi possível abrir o arquivo Vetor_B.bin para escrita\n");
        exit(3);
    }
    for(int i=0; i<nIncognitas; i++) {
        valor = (data_t)rand()/(data_t)(RAND_MAX/MAXIMO);
        fwrite(&valor, sizeof valor, 1, arq);
    }
    fclose(arq);

    return 0;
}
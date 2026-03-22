#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>

#include "comum.h"
#include "gpu.h"

/*
 * Variáveis globais
*/
// Número de incógnitas do sistema (igual ao número de linhas ou colunas da matriz A ou tamanho do vetor B)
int nIncognitas;
// Nome do arquivo contendo a matriz A
char *filenameMatrizA;
// Nome do arquivo contendo o vetor B
char *filenameVetorB;
// Nome do arquivo com a saída de dados
char *filenameTempos = (char *)"tempos.csv";
// Quantidade de threads por bloco
int threadsPerBlock = DEFAULT_THREADS_PER_BLOCK;
// Quantidade de blocos por grid
int blocksPerGrid = DEFAULT_MAX_BLOCKS_PER_GRID;
// Número máximo de valores ao exibir matrizes ou vetores
int maxPrint = MAX_PRINT;

int main(int argc, char *argv[]) {
    // Ax = B
    data_t *hmA, *hvB, *hvX;
    FILE *arq;

    // medida de tempo
    clock_t start_time, stop_time;

    parseArguments(argc, argv);

    /* Deprecated
    // verifica os argumentos
    if(argc != ARGC) {
        fprintf(stderr, "uso: %s <qtd incógnitas> <matriz A> <vetor B>\n", argv[0]);
        exit(3);
    }

    nIncognitas = atoi(argv[ARG_N_INCOGNITAS]);
    */
    // Lê os dados dos arquivos
    // Matriz A
    hmA = leMatriz(filenameMatrizA, nIncognitas);
    hvB = leVetor(filenameVetorB, nIncognitas);

    fprintf(stderr, "Vetores lidos dos arquivos:\n");
    exibeMatriz(hmA, nIncognitas);
    exibeVetor(hvB, nIncognitas);

    // liga o cronômetro
    start_time = clock();
    
    processaVetores(hmA, hvB, nIncognitas);

    // desliga o cronômetro
    stop_time = clock();

    // Show init exec time
    printf("Tempo do cálculo: %f ms\n", timedifference_msec(start_time, stop_time));
    if(!(arq = fopen("tempos.csv", "a"))) {
        fprintf(stderr, "Não foi possível abrir o arquivo de tempos.csv para incluir um novo tempo\n");
        exit(15);
    }
    fprintf(arq, "%s,%f,", argv[0], timedifference_msec(start_time, stop_time));
    fclose(arq);

    fprintf(stderr, "Vetores transformados:\n");
    exibeMatriz(hmA, nIncognitas);
    exibeVetor(hvB, nIncognitas);

    hvX = calculaX(hmA, hvB, nIncognitas);

    // verifica o resultado
    hmA = leMatriz(filenameMatrizA, nIncognitas);
    hvB = leVetor(filenameVetorB, nIncognitas);
    verificaResultado(hmA, hvX, hvB, nIncognitas);

    // Libera memória
    free(hmA);
    free(hvB);
    free(hvX);

    return 0;
}

/**
 * Função parseArguments: realiza o parse de argumentos da linha de comando.
 * Argumentos válidos:
 * -m <nome do arquivo com a matriz A>
 * -n <quantidade de incógnitas>
 * -o <nome do arquivo de saída de tempos> (opcional - default tempos.csv)
 * -v <nome do arquivo com o vetor B>
 * -t <número de threads por bloco> (opcional - default DEFAULT_THREADS_PER_BLOCK)
 * -g <número de blocos por grid> (opcional - default DEFAULT_MAX_BLOCKS_PER_GRID)
 * 
 * @param argc  número de argumentos
 * @param argv  vetor com os argumentos
*/
void parseArguments(int argc, char *argv[]) {
    opterr = 0;
    int opcao;      // opção selecionada
    char *shortOpts = (char *)"g:m:n:o:p:v:";
    int flagFilenameMatrizA = FALSE;
    int flagFilenameVetorB = FALSE;
    int flagNIncognitas = FALSE;

    while((opcao=getopt(argc, argv, shortOpts)) != -1) {
        switch (opcao) {
        case 'g':
            threadsPerBlock = atoi(optarg);
            break;

        case 'm':
            filenameMatrizA = optarg;
            flagFilenameMatrizA = TRUE;
            break;

        case 'n':
            nIncognitas = atoi(optarg);
            flagNIncognitas = TRUE;
            break;

        case 'o':            // opcional
            filenameTempos = optarg;
            break;

        case 'p':
            maxPrint = atoi(optarg);
            break;

        case 't':
            blocksPerGrid = atoi(optarg);
            break;

        case 'v':
            filenameVetorB = optarg;
            flagFilenameVetorB = TRUE;
            break;
        
        default:
            fprintf(stderr, "Opção desconhecida: %c\n", optopt);
            break;
        }
    }
    if(!flagFilenameMatrizA) fprintf(stderr, "Nome do arquivo com a matriz A faltando (opção -m)\n");
    if(!flagNIncognitas)     fprintf(stderr, "Número de incógnitas do sistema faltando (opção -n)\n");
    if(!flagFilenameVetorB)  fprintf(stderr, "Nome do arquivo com o vetor B faltando (opção -v)\n");
    if(!flagFilenameMatrizA || !flagFilenameVetorB || !flagNIncognitas) {
        uso(argv[0]);
        exit(-1);
    }
}

void uso(char *nome) {
    fprintf(stderr, "Uso: %s -m <arquivo com a matriz A> -n <número de incógnitas do sistema> [-o <nome do arquivo de tempos>] -v <nome do arquivo com o vetor B>\n", nome);
}
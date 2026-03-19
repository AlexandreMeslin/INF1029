/*
GPU
Versão Convencional:
- Transforma a matriz A e o vetor B de forma convencional

Versão 1: 
- Baseado na ideia original com tentativa de sincronização

Versão 2:
- Sem necessidade de sincronização porque utiliza um vetor auxiliar para armazenar os dados compartilhados entre as threads

Versão 3:
- Usa uma matriz auxiliar para armazenar o resultado

Versão 4:
- Distribuiu uniformemente os elementos da matriz pelos processadores
*/

#ifndef COMUM
#define COMUM

// macro de acesso à matriz
#define matriz(mA,linha,coluna,tamanho) (mA[(linha)*(tamanho)+(coluna)])
// calcula o menor de 2 números
#define menor(a,b)  (((a)<(b))? (a):(b))

// número de elementos máximo a serem impressos (linha e coluna)
#define MAX_PRINT   7

// erro máximo aceitável
#define ERRO_MAXIMO 0.01

// Constantes booleanas
#define FALSE 0
#define TRUE -1

// tipos de dados
typedef float data_t;

// Macros
#define timedifference_msec(start, stop) ((double)(stop - start) * 1000.0 / CLOCKS_PER_SEC)

// Protótipos
void exibeMatriz(data_t *hmA, int nIncognitas);
void exibeVetor(data_t *hvB, int nIncognitas);
data_t *leMatriz(char *nome, int nIncognitas);
data_t *leVetor(char *nome, int nIncognitas);
data_t *calculaX(data_t *hmA, data_t *hvB, int nIncognitas);
void verificaResultado(data_t *hmA, data_t *hvX, data_t *hvB, int nIncognitas);
void exibeLinhaMatriz(data_t *hmA, int linha, int nIncognitas);
void processaVetores(data_t *hmA, data_t *hvB, int nIncognitas);
void parseArguments(int argc, char *argv[]);
void uso(char *nome);

// variáveis globais
extern int nIncognitas;
extern char *filenameMatrizA;
extern char *filenameVetorB;
extern char *filenameTempos;
extern int threadsPerBlock;
extern int blocksPerGrid;
extern int maxPrint;

#endif
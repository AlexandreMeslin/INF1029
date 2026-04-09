#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/* This global structure will be accesse by the threads.
 * The arrays (a and b) will be split in slices for each thread.
 * The sum field is the critical region and will be protected by
 * a mutex variable (semaphore).
 */
typedef struct
{
    double *a;
    double *b;
    double sum;
    int slicelen;
} DOTDATA;

/* Define globally accessible variables and a mutex */
#define NUMTHRDS 8
#define PROBLEMLEN 100000000
long SLICELEN = PROBLEMLEN / NUMTHRDS;
DOTDATA dotstr;
pthread_t threads[NUMTHRDS];
pthread_mutex_t mutexsum;

/* This function will be executed by each thread. It calculates
 * the product of each element of a slice of the arrays a and b
 * and calculates the sum (mysum) of the result of each product.
 * Then, the critical region is accessed in order to update the
 * global sum.
 */
void *dotprod(void *arg) {
    int i, start, end, len;
    long offset;
    double mysum;
    double *x, *y;
    /* Calculate the boundary of the slice of this thread */
    offset = (long)arg;
    len = dotstr.slicelen;
    start = offset * len;
    end = start + len;
    x = dotstr.a;
    y = dotstr.b;
    //dotstr.sum = 0;   // deve ser inicializado antes de criar as threads
    mysum = 0;
    for (i = start; i < end; i++) {
        //pthread_mutex_lock(&mutexsum);
        mysum += (x[i] * y[i]);
        //pthread_mutex_unlock(&mutexsum);
    }
    /* Lock a mutex prior to updating the value in the shared
     * structure, and unlock it upon updating.
     */
    pthread_mutex_lock(&mutexsum);
    dotstr.sum += mysum;
    pthread_mutex_unlock(&mutexsum);
    pthread_exit((void *)0);
}

int main(void) {
    int rc; /// return code for pthread functions
    long i;
    double *a, *b;
    void *status;
    pthread_attr_t attr;
    clock_t tempoInicial;
    clock_t tempoFinal;
    double tempoDecorrido;

    tempoInicial = clock();
    /* Assign storage and initialize values */
    a = (double *)malloc(PROBLEMLEN * sizeof(double));
    b = (double *)malloc(PROBLEMLEN * sizeof(double));
    for (i = 0; i < PROBLEMLEN; i++) {
        a[i] = 1.0;
        b[i] = a[i];
    }
    dotstr.slicelen = SLICELEN;
    dotstr.a = a;
    dotstr.b = b;
    dotstr.sum = 0;
    pthread_mutex_init(&mutexsum, NULL);
    /* Create threads to perform the dotproduct */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    for (i = 0; i < NUMTHRDS; i++)
    {
        /* Each thread works on a different set of data. The offset is specified
         * by 'i'. The size of the data for each thread is indicated by SLICELEN.
         */
        if ((rc = pthread_create(&threads[i], &attr, dotprod, (void *)i)))
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    pthread_attr_destroy(&attr);
    /* Wait on the other threads */
    for (i = 0; i < NUMTHRDS; i++)
    {
        if ((rc = pthread_join(threads[i], &status)))
        {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
    }
    tempoFinal = clock();
    tempoDecorrido = (tempoFinal - tempoInicial) / CLOCKS_PER_SEC * 1000;
    /* After joining, print out the results and cleanup */
    printf("Sum = %f em %lf s\n", dotstr.sum, tempoDecorrido);
    free(a);
    free(b);
    pthread_mutex_destroy(&mutexsum);
    pthread_exit(NULL);
}
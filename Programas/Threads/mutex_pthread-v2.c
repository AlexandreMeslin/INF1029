/**
 * @file mutex_pthread.c
 * 
 * Esta é a versão 2 do programa mutex_pthread.c, que calcula o produto escalar de dois vetores usando threads.
 * Nessa versão, o programa foi modificado para alocar sempre a mesma quantidade de memória para os vetores, independentemente do número de threads. 
 * Cada thread calcula uma porção do produto escalar e atualiza uma variável de soma compartilhada.
 * 
 * This program calculates the dot product of two vectors using threads.
 * Each thread calculates a portion of the dot product and updates a shared sum variable.
 * A mutex is used to protect the critical section where the shared sum variable is updated.
 * Compile with: gcc -Wall -Wextra -o mutex_pthread mutex_pthread.c -lpthread
 * Run with: ./mutex_pthread
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * This global structure will be accesse by the threads.
 * The arrays (a and b) will be split in slices for each thread.
 * The sum field is the critical region and will be protected by
 * a mutex variable (semaphore).
 */
typedef struct {
   double *a;
   double *b;
   double sum;
   int slicelen;
} DOTDATA;

/* 
 * Define globally accessible variables and a mutex. 
 * The number of threads and the length of the slice of the arrays that each thread will process are defined as constants.
 */
#define NUMTHRDS 1
#define PROBLEMLEN 1000000000
#define SLICELEN (PROBLEMLEN / NUMTHRDS)

DOTDATA dotstr;
pthread_t threads[NUMTHRDS];
pthread_mutex_t mutexsum;

/**
 * This function will be executed by each thread. It calculates
 * the product of each element of a slice of the arrays a and b
 * and calculates the sum (mysum) of the result of each product.
 * Then, the critical region is accessed in order to update the
 * global sum.
 */

void *dotprod(void *arg) {
   int i, start, end, len;
   long offset;
   double mysum, *x, *y;

   /* Calculate the boundary of the slice of this thread */
   offset = (long)arg;

   len = dotstr.slicelen;
   start = offset * len;
   end = start + len;
   x = dotstr.a;
   y = dotstr.b;

   mysum = 0;
   for (i = start; i < end; i++) mysum += (x[i] * y[i]);

   /* 
    * Lock a mutex prior to updating the value in the shared
    * structure, and unlock it upon updating.
    */
   pthread_mutex_lock(&mutexsum);
   dotstr.sum += mysum;
   pthread_mutex_unlock(&mutexsum);

   pthread_exit((void *)0);
}

/**
 * The main function initializes the data, creates the threads 
 * to perform the dot product calculation, and waits for the 
 * threads to complete before printing the result and cleaning 
 * up resources.
 * The main function also measures the time taken to perform the
 * dot product calculation and prints the elapsed time along 
 * with the final sum.
 * The main function does not return a value, as it calls 
 * pthread_exit to wait for all threads to finish before exiting
 * the program.
 * 
 * @return 0 The main function does not return a value, as it calls pthread_exit to wait for all threads to finish before exiting the program.
 */
int main(void) {
   int rc;
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

   dotstr.slicelen = SLICELEN;   // Define the length of the slice of the arrays that each thread will process
   dotstr.a = a;                 // Assign the pointer to the first array to the global structure
   dotstr.b = b;                 // Assign the pointer to the second array to the global structure
   dotstr.sum = 0;               // Initialize the sum field of the global structure to 0

   // O mutex é inicializado com atributos padrão, passando NULL como segundo argumento para pthread_mutex_init.
   // O mutext é inicializado destravado.
   pthread_mutex_init(&mutexsum, NULL);

   /* Create threads to perform the dotproduct  */
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
   for (i = 0; i < NUMTHRDS; i++) {
      /* 
       * Each thread works on a different set of data. The offset is specified
       * by 'i'. The size of the data for each thread is indicated by SLICELEN.
       */
      if ((rc = pthread_create(&threads[i], &attr, dotprod, (void *)i))) {
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
   }

   pthread_attr_destroy(&attr);

   /* Wait on the other threads */
   for (i = 0; i < NUMTHRDS; i++) {
      if ((rc = pthread_join(threads[i], &status))) {
         printf("ERROR; return code from pthread_join() is %d\n", rc);
         exit(-1);
      }
   }

   tempoFinal = clock();
   tempoDecorrido = (tempoFinal - tempoInicial) / CLOCKS_PER_SEC * 1000;

   /* After joining, print out the results and cleanup */
   printf("Sum =  %f em %lf ms\n", dotstr.sum, tempoDecorrido);
   free(a);
   free(b);
   pthread_mutex_destroy(&mutexsum);

   pthread_exit(NULL);
}

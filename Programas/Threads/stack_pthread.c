/**
 * @file stack_pthread.c
 * @brief A simple program that demonstrates how to set the stack size for threads using the pthread library in C.
 *
 * This program demonstrates how to set the stack size for threads using the pthread library in C.
 * Each thread performs a simple computation that requires a large stack size, and the program shows how to calculate the required stack size and set it using pthread_attr_setstacksize.
 * Compile with: gcc -Wall -Wextra -o stack_pthread stack_pthread.c -lpthread
 * Run with: ./stack_pthread
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 4

#define N 1024
#define MEGEXTRA 1024000

pthread_attr_t attr;

/**
 * @brief The function executed by each thread to perform a computation that requires a large stack size.
 *
 * The function to be executed by each thread.
 * It takes a single argument,
 * which is the thread ID,
 * and performs a simple computation that requires a large stack size by creating a large 2D array and filling it with values.
 * After completing the computation, it prints the stack size used by the thread and calls pthread_exit to terminate the thread.
 *
 * @param threadid A pointer to the thread ID, which is cast to a long integer for printing.
 * @return NULL The function does not return any value, as it calls pthread_exit to terminate the thread.
 */
void *dowork(void *threadid)
{
   double A[N][N]; // a pior forma de alocar um array, mas é para forçar o uso de uma pilha grande
   int i, j;
   long tid;
   size_t mystacksize;

   tid = (long)threadid;
   pthread_attr_getstacksize(&attr, &mystacksize);
   printf("Thread %ld: stack size = %li bytes \n", tid, mystacksize);

   for (i = 0; i < N; i++) {
      for (j = 0; j < N; j++) {
         A[i][j] = ((i * j) / 3.452) + (N - i);
      }
      if(!(i%1000)) {
         printf("Thread %ld: completed row %d A[%d][%d] = %f\n", tid, i, i, j, A[i][0]);
      }
   }

   pthread_exit(NULL);
}

/**
 * @brief The main function that creates multiple threads to execute the dowork function with a specified stack size.
 * 
 * The main function initializes a thread attribute structure, calculates the required stack size for each thread based on the size of the 2D array and an additional buffer, and sets the stack size using pthread_attr_setstacksize.
 * It then creates a specified number of threads (defined by NTHREADS) and assigns each thread to execute the dowork function. 
 * Each thread will print the stack size it is using. After creating all threads, the main function calls pthread_exit to wait for all threads to complete before terminating the program.
 * 
 * @return 0 The main function does not return a value, as it calls pthread_exit to wait for all threads to finish before exiting the program.
 */
int main(void) {
   pthread_t threads[NTHREADS];
   size_t stacksize;
   int rc;
   long t;

   pthread_attr_init(&attr);
   pthread_attr_getstacksize(&attr, &stacksize);
   printf("Default stack size = %li\n", stacksize);

   stacksize = sizeof(double) * N * N + MEGEXTRA;
   printf("Amount of stack needed per thread = %li\n", stacksize);

   pthread_attr_setstacksize(&attr, stacksize);
   printf("Creating threads with stack size = %li bytes\n", stacksize);

   for (t = 0; t < NTHREADS; t++) {
      rc = pthread_create(&threads[t], &attr, dowork, (void *)t);
      if (rc) {
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
   }
   printf("Created %ld threads.\n", t);

   pthread_exit(NULL);
}

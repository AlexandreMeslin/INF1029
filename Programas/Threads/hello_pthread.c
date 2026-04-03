/**
 * @file hello_pthread.c
 * @brief A simple program that creates several threads, and each thread prints a message.
 * 
 * A simple program that creates several threads, and each thread prints a message.
 * Compile with: gcc -Wall -Wextra -o hello_pthread hello_pthread.c -lpthread
 * Run with: ./hello_pthread
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS     5

/**
 * @brief The function executed by each thread to print a message.
 * 
 * The function to be executed by each thread. 
 * It takes a single argument, which is the thread ID, and prints a message including that ID. 
 * After printing the message, it calls pthread_exit to terminate the thread.
 * 
 * @param threadid A pointer to the thread ID, which is cast to a long integer for printing.
 * @return NULL The function does not return any value, as it calls pthread_exit to terminate
 */
void *printHello(void *threadid) {
    long tid;
    tid = (long)threadid;
    printf("Hello World! It's me, thread #%ld!\n", tid);
    pthread_exit(NULL);
}

/**
 * @brief The main function that creates multiple threads to execute the printHello function.
 * 
 * The main function creates a specified number of threads (defined by NUM_THREADS) and assigns each thread to execute the printHello function. 
 * It prints a message each time a thread is created, indicating the thread number. 
 * After creating all threads, the main function calls pthread_exit to wait for all threads to complete before terminating the program.
 * 
 * @return 0 The main function does not return a value, as it calls pthread_exit to wait for all threads to finish before exiting the program.
 */
int main (void) {
    pthread_t threads[NUM_THREADS]; /// Array to hold thread identifiers
    int rc;                         /// Variable to hold return code from pthread_create

    // Create threads to execute the printHello function
    for(long t=0; t<NUM_THREADS; t++){
       printf("In main: creating thread %ld\n", t);

       // threads[t] retorna o identificador da thread criada
       // NULL means default thread attributes
       // printHello is the function to be executed by the thread
       // (void *)t is the argument passed to the thread function, cast to void pointer
       rc = pthread_create(&threads[t], NULL, printHello, (void *)t);
       if (rc){
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
       }
    }

    /* Last thing that main() should do */
    pthread_exit(NULL);
}

/**
 * @file join_pthread.c
 * @brief A simple program that creates several threads, each performing a busy work, and waits for them to complete using pthread_join.
 * 
 * This program demonstrates how to create threads, perform some work in each thread, and then wait for all threads to finish before exiting the main program.
 * Compile with: gcc -Wall -Wextra -o join_pthread join_pthread.c -lpthread -lm
 * Run with: ./join_pthread
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_THREADS     4

/**
 * @brief The function executed by each thread to perform a busy work.
 * 
 * The function to be executed by each thread. 
 * It takes a single argument, which is the thread ID, and performs a busy work by calculating a result using sine and tangent functions in a loop. 
 * After completing the work, it prints the result and calls pthread_exit to terminate the thread, returning the thread ID as the exit status.
 * 
 * @param t A pointer to the thread ID, which is cast to a long integer for printing and returning as the exit status.
 * @return NULL The function does not return a value, as it calls pthread_exit to terminate the thread and return the thread ID as the exit status.
 */
void *busyWork(void *t) {
    long tid;
    double result=0.0;

    tid = (long)t;
    printf("Thread %ld starting...\n",tid);
    for (int i=0; i<100000000; i++) {
       result = result + sin(i) * tan(i);
    }
    printf("Thread %ld done. Result = %e\n",tid, result);
    pthread_exit((void*) t);
}

/**
 * @brief The main function that creates multiple threads to execute the busyWork function and waits for them to complete.
 *
 * The main function creates a specified number of threads (defined by NUM_THREADS) and assigns each thread to execute the busyWork function.
 * It initializes a thread attribute structure and sets it to joinable, allowing the main thread to wait for each created thread to complete.
 * After creating all threads, the main function calls pthread_join to wait for each thread to finish before terminating the program.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line arguments.
 * @return 0 The main function does not return a value, as it calls pthread_exit to terminate the program.
 */
int main (void) {
    pthread_t thread[NUM_THREADS];
    pthread_attr_t attr;
    int rc;
    void *status;

    /* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for(long t=0; t<NUM_THREADS; t++) {
       printf("Main: creating thread %ld\n", t);
       // thread[t] is the thread identifier returned by pthread_create
       // &attr is the thread attributes, set to joinable
       // busyWork is the function to be executed by the thread
       // (void *)t is the argument passed to the thread function, cast to void pointer
       rc = pthread_create(&thread[t], &attr, busyWork, (void *)t);
       if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    /* Free attribute and wait for the other threads */
    pthread_attr_destroy(&attr);

    for(long t=0; t<NUM_THREADS; t++) {
       rc = pthread_join(thread[t], &status);
       if (rc) {
          printf("ERROR; return code from pthread_join() is %d\n", rc);
          exit(-1);
        }
        printf("Main: completed join with thread %ld having a status of %ld\n",t,(long)status);
    }

    printf("Main: program completed. Exiting.\n");
    pthread_exit(NULL);
}

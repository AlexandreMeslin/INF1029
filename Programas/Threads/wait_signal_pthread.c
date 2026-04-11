/**
 * @file wait_signal_pthread.c
 * FILE: condvar.c
 * DESCRIPTION:
 *   Example code for using Pthreads condition variables.  The main thread
 *   creates three threads.  Two of those threads increment a "count" variable,
 *   while the third thread watches the value of "count".  When "count" 
 *   reaches a predefined limit, the waiting thread is signaled by one of the
 *   incrementing threads. The waiting thread "awakens" and then modifies
 *   count. The program continues until the incrementing threads reach
 *   TCOUNT. The main program prints the final value of count.
 * SOURCE: Adapted from example code in "Pthreads Programming", B. Nichols
 *   et al. O'Reilly and Associates. 
 * LAST REVISED: 10/14/10  Blaise Barney
 * To compile the program, use the following command: gcc -Wall -Wextra -o wait_signal_pthread wait_signal_pthread.c -lpthread
 * To run the program, use the command: ./wait_signal_pthread
 * 
 * @author  B. Nichols et al.
 * @version 1.0
 * @date    2010-10-14
 * @copyright 2010 O'Reilly and Associates, Inc. All rights reserved.
 * @note    This program is intended for educational purposes to demonstrate the use of condition variables and mutexes in C with pthreads.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS  3
#define TCOUNT 10
#define COUNT_LIMIT 12

int     count = 0;                        // Quantidade de itens produzidos ainda não consumidos.
long    thread_ids[3] = {0,1,2};          // array holds the thread IDs for the three threads created in the main function. The IDs are used to identify the threads when printing messages about their actions.
pthread_mutex_t count_mutex;              // mutex is used to protect access to the count variable, ensuring that only one thread can modify or read the count variable at a time to avoid race conditions.
pthread_cond_t count_threshold_cond_var;  // condition variable is used to signal the waiting thread (watch_count) when the count variable reaches the specified limit (COUNT_LIMIT). The inc_count threads signal this condition variable when they update the count variable and it reaches the limit.

/**
 * This function is executed by the threads that increment the count variable.
 * Each thread locks the mutex, increments the count variable, 
 * checks if the count has reached the specified limit, 
 * and signals the waiting thread if the limit is reached. 
 * The mutex is unlocked after the count variable is updated, 
 * allowing other threads to access it. 
 * The function also includes a sleep call to allow threads to alternate on the mutex lock, 
 * demonstrating the synchronization between threads.
 * 
 * @param t The thread ID passed as an argument to the function, used for printing the thread's actions.
 * @return NULL The function does not return a value, as it calls pthread_exit to terminate the thread after completing its task.
 */
void *inc_count(void *t) {
   int i;
   long my_id = (long)t;

   for (i=0; i<TCOUNT; i++) {
      pthread_mutex_lock(&count_mutex);
      count++;

      /* 
       * Check the value of count and signal waiting thread when condition is
       * reached.  Note that this occurs while mutex is locked. 
       */
      if (count == COUNT_LIMIT) {
         pthread_cond_signal(&count_threshold_cond_var);
         printf("inc_count(): thread %ld, count = %d  Threshold reached.\n", my_id, count);
      }
      printf("inc_count(): thread %ld, count = %d, unlocking mutex\n", my_id, count);
      pthread_mutex_unlock(&count_mutex);

      /* Hold on for 1 second so threads can alternate on mutex lock */
      sleep(1);
   }
   pthread_exit(NULL);
}

/**
 * This function is executed by the thread that waits for the count variable to reach a specified limit.
 * The thread locks the mutex and waits for the condition variable to be signaled when the count variable reaches the limit. 
 * The function includes a loop to check the count variable before waiting, 
 * ensuring that the thread does not wait indefinitely if the count variable has already reached the limit before the thread starts waiting. 
 * Once the condition is met and the thread is signaled, 
 * it updates the count variable and prints the final value before unlocking the mutex and exiting.
 * 
 * @param t The thread ID passed as an argument to the function, used for printing the thread's actions.
 * @return NULL The function does not return a value, as it calls pthread_exit to terminate the thread after completing its task.
 * @note The function demonstrates the use of condition variables to synchronize threads based on a shared variable 
 * and the importance of checking the condition before waiting to avoid missed signals. 
 * The function also shows how to properly lock and unlock mutexes when accessing shared variables to ensure thread safety.
 */
void *watch_count(void *t) {
   long my_id = (long)t;

   printf("Starting watch_count(): thread %ld\n", my_id);

   /*
    * Lock mutex and wait for signal.  Note that the pthread_cond_wait 
    * routine will automatically and atomically unlock mutex while it waits. 
    * Also, note that if COUNT_LIMIT is reached before this routine is run by
    * the waiting thread, the loop will be skipped to prevent pthread_cond_wait
    * from never returning. 
    */
   pthread_mutex_lock(&count_mutex);
   while (count<COUNT_LIMIT) {
      printf("watch_count(): Vou começar a esperar...\n");
      // entra em wait e libera o mutex, permitindo que outras threads acessem o count_mutex e modifiquem a variável count.
      // Quando a condição é sinalizada por pthread_cond_signal, a thread é despertada e tenta adquirir o mutex novamente para continuar a execução. 
      // Se o count ainda não atingiu o COUNT_LIMIT, ela volta a esperar, liberando o mutex novamente.
      pthread_cond_wait(&count_threshold_cond_var, &count_mutex);
      printf("watch_count(): thread %ld Condition signal received.\n", my_id);
   }
   count += 125;
   printf("watch_count(): thread %ld count now = %d.\n", my_id, count);
   pthread_mutex_unlock(&count_mutex);
   pthread_exit(NULL);
}

/**
 * The main function initializes the mutex and condition variable, creates the threads to perform their respective tasks, and waits for all threads to complete before cleaning up resources and exiting the program. 
 * The main function also includes error checking for thread creation and joining, ensuring that any issues with thread management are properly handled. 
 * After all threads have completed their tasks, the main function prints a message indicating that it has waited on all threads and is done, before destroying the mutex and condition variable and exiting the program.
 * 
 * @return 0 The main function does not return a value, as it calls pthread_exit to wait for all threads to finish before exiting the program.
 * @note The main function demonstrates the proper initialization and cleanup of mutexes and condition variables,
 */
int main (void) {
   int i, rc;
   pthread_t threads[3];
   pthread_attr_t attr;

   /* Initialize mutex and condition variable objects */
   pthread_mutex_init(&count_mutex, NULL);
   pthread_cond_init (&count_threshold_cond_var, NULL);

   /* For portability, explicitly create threads in a joinable state */
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
   if ((rc = pthread_create(&threads[0], &attr, watch_count, (void *)thread_ids[0]))) {
	printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
   }
   if ((rc = pthread_create(&threads[1], &attr, inc_count, (void *)thread_ids[1]))) {
	printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
   }
   if ((rc = pthread_create(&threads[2], &attr, inc_count, (void *)thread_ids[2]))) {
	printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
   }

   /* Wait for all threads to complete */
   for (i=0; i<NUM_THREADS; i++) {
     if ((rc = pthread_join(threads[i], NULL))) {
	printf("ERROR; return code from pthread_join() is %d\n", rc);
        exit(-1);
     }
   }
   printf ("Main(): Waited on %d  threads. Done.\n", NUM_THREADS);

   /* Clean up and exit */
   pthread_attr_destroy(&attr);
   pthread_mutex_destroy(&count_mutex);
   pthread_cond_destroy(&count_threshold_cond_var);
   pthread_exit(NULL);
}

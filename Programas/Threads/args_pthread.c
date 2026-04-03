/**
 * @file args_pthread.c
 * @brief A simple program that demonstrates how to pass arguments to threads using the pthread library in C.
 * 
 * This program demonstrates how to pass arguments to threads using the pthread library in C.
 * Each thread receives a structure containing its ID, an offset, and a message to print.
 * Compile with: gcc -Wall -Wextra -o args_pthread args_pthread.c -lpthread
 * Run with: ./args_pthread
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 5

struct thread_data
{
   long thread_id;
   int offset;
   char *message;
};

/**
 * @brief The function executed by each thread to print a message with an offset.
 *
 * The function to be executed by each thread.
 * It takes a pointer to a thread_data structure as an argument,
 * which contains the thread ID, an offset, and a message.
 * The function prints a message that includes the thread ID and a portion of the message string based on the offset.
 * After printing the message, it calls pthread_exit to terminate the thread.
 *
 * @param threadarg A pointer to the thread data structure.
 * @return NULL The function does not return any value.
 */
void *printHello(void *threadarg)
{
   struct thread_data *my_data;

   // Transforma o ponteiro genérico em um ponteiro para a estrutura thread_data
   my_data = (struct thread_data *)threadarg;

   printf("Thread #%ld: %s\n", my_data->thread_id, my_data->message + my_data->offset);

   /* a última coisa que toda thread deve fazer */
   pthread_exit(NULL);
}

/**
 * @brief The main function that creates multiple threads to execute the printHello function with arguments.
 *
 * The main function creates a specified number of threads (defined by NUM_THREADS) and assigns each thread to execute the printHello function.
 * It initializes a thread_data structure for each thread, which includes the thread ID, an offset, and a message string.
 * Each thread will print a portion of the message string based on its offset.
 * After creating all threads, the main function calls pthread_exit to wait for all threads to complete before terminating the program.
 *
 * @return 0 The main function does not return a value, as it calls pthread_exit to wait for all threads to finish before exiting the program.
 */
int main(void)
{
   pthread_t threads[NUM_THREADS];
   int rc;
   struct thread_data thread_data_array[NUM_THREADS];
   char message[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

   for (long t = 0; t < NUM_THREADS; t++) {
      printf("In main: creating thread %ld\n", t);
      thread_data_array[t].thread_id = t;
      thread_data_array[t].offset = t * 4;
      thread_data_array[t].message = message;

      // threads[t] is the thread identifier
      // NULL means default thread attributes
      // printHello is the function to be executed by the thread
      // (void *)&thread_data_array[t] is the argument passed to the thread function, cast to void pointer
      rc = pthread_create(&threads[t], NULL, printHello, (void *)&thread_data_array[t]);
      if (rc) {
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
   }

   /* Last thing that main() should do */
   pthread_exit(NULL);
}

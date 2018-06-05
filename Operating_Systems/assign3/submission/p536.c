/*
 *  pid_manager.c
 *
 *  Created on: Feb 6,2018
 *  Author: Alex Lay
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include "pid_api.h"
#include "int.h"

#define NUM_THREADS 100
pthread_mutex_t mutex;
int counter = 0;  // race condition

void createThreads(pthread_t *);
void joinThreads(pthread_t *);
void *runner(void *param);
void modifyCounter();

/** Programming Exercise 3.20 required you to design a PID manager that
  * allocated a unique process identifier to each process. Exercise 4.20
  * required you to modify your solution to Exercise 3.20 by writing a
  * program that created a number of threads that requested and released
  * process identifiers. Now modify your solution to Exercise 4.20 by
  * ensuring that the data structure used to represent the availability of
  * process identifiers is safe from race conditions. Use Pthreads mutex
  * locks, described in Section 5.9.4.
  */
int main() {
  srand(time(NULL));
  pthread_t tid[NUM_THREADS];
  pthread_mutex_init(&mutex,NULL);
  int can_allocate = allocate_map();
  if(can_allocate == -1)
    printf("Can't allocate bitmap.\n");

  /*printf("Before creating threads:\n");
  print_pids();*/

  createThreads(tid);
  /*printf("In between create & join threads\n");
  print_pids();*/
  joinThreads(tid);

  /*printf("After creating threads:\n");
  print_pids();*/
  return 0;
}
/** Creates NUM_THREADS amount of threads.
  *
  * Each thread requests a pid from the bitmap
  * The pid is then passed to the thread in a runner function.
  *
  * @param tid = An array of tid's of size NUM_THREADS
  */
void createThreads(pthread_t *tid) {
  int requested_pid = 1;
  for(int i = 0; i < NUM_THREADS; i++) {
    requested_pid = allocate_pid();
    if(requested_pid == 1)
      printf("Couldn't allocate pid!\n");
    else {
      pthread_create(&tid[i],NULL,runner,newINT(requested_pid));
    }
  }
}
/** Joins threads after they have been created.
  *
  * @param tid = An array of tid's of size NUM_THREADS
  */
void joinThreads(pthread_t *tid) {
  for(int i = 0; i < NUM_THREADS; i++) {
    pthread_join(tid[i],NULL);
  }
}
/** Each thread begins control in this function
  *
  * Each thread waits for a random amount of time
  * It then releases the pid associated with it and
  *   exits the thread.
  * Now this function uses a mutex lock to prevent
  *   multiple threads from modifying the counter at
  *   the same time.
  *
  * @param param = The pid (cast as an INT)
  */
void *runner(void *param) {
  int pid = getINT(param);
  pthread_t tid = pthread_self();
  int time_sleep = rand() % 15; // Modded b/c I don't want to wait forever
  printf("Sleeping Time: %d secs; Thread ID = %lu; Counter Value = %d.\n",
    time_sleep,tid,counter);
  sleep(time_sleep);
  pthread_mutex_lock(&mutex);  // acquire
  /* critical section */
  modifyCounter();
  /* end critical section */
  pthread_mutex_unlock(&mutex); // release
  // Release pid is not part of the critical section b/c each thread
  // is mapped to a separate part of the bitmap. Therefore, they don't
  // modify the same portion of the bitmap.
  release_pid(pid);
  pthread_exit(0);
}
/** Randomly increments or decrements the global counter variable.
  * This is the critical section for the threads as the counter
  *   variable is shared across all threads.
  */
void modifyCounter() {
  int increment = rand() % 2;
  if(increment)
    counter++;
  else
    counter--;
  printf("Modifying. Counter value is now %d\n",counter);
}

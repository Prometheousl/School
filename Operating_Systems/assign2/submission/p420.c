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

void createThreads(pthread_t *);
void joinThreads(pthread_t *);
void *runner(void *param);

/** Modify programming problem Exercise 3.20 from Chapter 3, which asks
  * you to design a pid manager. This modification will consist of writing
  * a multithreaded program that tests your solution to Exercise 3.20. You
  * will create a number of threads—for example, 100—and each threadwill
  * request a pid, sleep for a random period of time, and then release the pid.
  * (Sleeping for a random period of time approximates the typical pid usage
  * in which a pid is assigned to a new process, the process executes and
  * then terminates, and the pid is released on the process’s termination.) On
  * UNIX and Linux systems, sleeping is accomplished through the sleep()
  * function, which is passed an integer value representing the number of
  * seconds to sleep. This problem will be modified in Chapter 5.
  */
int main() {
  pthread_t tid[NUM_THREADS];
  int can_allocate = allocate_map();
  if(can_allocate == -1)
    printf("Can't allocate bitmap.\n");
  srand(time(NULL));

  // The blackboard specs said to put printf's in so I did...
  printf("Before creating threads:\n");
  print_pids();

  createThreads(tid);
  printf("In between create & join threads\n");
  print_pids();
  joinThreads(tid);

  printf("After creating threads:\n");
  print_pids();
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
  printf("Creating threads!\n");
  int requested_pid = 1;
  for(int i = 0; i < NUM_THREADS; i++) {
    requested_pid = allocate_pid();
    if(requested_pid == 1)
      printf("Couldn't allocate pid!\n");
    else {
      printf("Allocating pid %d.\n",requested_pid);
      pthread_create(&tid[i],NULL,runner,newINT(requested_pid));
    }
  }
}
/** Joins threads after they have been created.
  *
  * @param tid = An array of tid's of size NUM_THREADS
  */
void joinThreads(pthread_t *tid) {
  printf("Joining threads!\n");
  for(int i = 0; i < NUM_THREADS; i++) {
    pthread_join(tid[i],NULL);
  }
}
/** Each thread begins control in this function
  *
  * Each thread waits for a random amount of time
  * It then releases the pid associated with it and
  *   exits the thread
  *
  * @param param = The pid (cast as an INT)
  */
void *runner(void *param) {
  int pid = getINT(param);
  pthread_t tid = pthread_self();
  printf("We got a runner! TID: %d\n",(int)tid);
  printf("PID is %d\n",pid);
  printf("About to sleep.\n");
  int time_sleep = rand() % 6; // Modded b/c I don't want to wait forever
  sleep(time_sleep);

  /* This would be the critical section for the threads but
   *   since each pid is assigned a different bit then they
   *   will never be shared. So, no lock is needed.
   */
   printf("Releasing pid %d!\n",pid);
  release_pid(pid);
  pthread_exit(0);
}

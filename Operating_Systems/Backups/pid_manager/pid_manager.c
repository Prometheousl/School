/** An operating system’s pid manager is responsible for managing process
  * identifiers. When a process is first created, it is assigned a unique pid
  * by the pid manager. The pid is returned to the pid manager when the
  * process completes execution, and the manager may later reassign this
  * pid. Process identifiers are discussed more fully in Section 3.3.1. What
  * is most important here is to recognize that process identifiers must be
  * unique; no two active processes can have the same pid.
  * Use the following constants to identify the range of possible pid values:
  * #define MIN PID 300
  * #define MAX PID 5000
  * You may use any data structure of your choice to represent the availability
  * of process identifiers. One strategy is to adopt what Linux has
  * done and use a bitmap in which a value of 0 at position i indicates that
  * a process id of value i is available and a value of 1 indicates that the
  * process id is currently in use.
  */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include "bitmap.h"

#define MIN_PID 300
#define MAX_PID 5000

#define NUM_THREADS 20

BITMAP *bm; // Have to declare out here b/c supposed to be API

int allocate_map(void);
int allocate_pid(void);
void release_pid(int pid);

void createThread();
/* The thread will begin control in this function */
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
  allocate_map();
  void *temp = (void*)"10";
  srand(time(NULL));


  for(int i = 0; i < NUM_THREADS; i++) {
    createThread();
  }
  for(int x = 0; x < 20; x++) {
    int pid = allocate_pid();
    printf("Allocated pid %d\n",pid);
  }
  release_pid(10);
  release_pid(11);
  printBITMAP(stdout,bm);
  return 0;
}
void createThread() {
  pthread_t pid = (pthread_t)allocate_pid();
  pthread_attr_t attr; /* set of thread attributes */
  /* get the default attributes */
  pthread_attr_init(&attr);
  /* create the thread */
  pthread_create(&pid,&attr,runner,NULL);
}
void joinThread(int pid)
/* The thread will begin control in this function */
void *runner(void *param) {
  pthread_t tid = pthread_self();
  printf("We got a runner! TID: %d\n",(int)tid);

  int pid = allocate_pid();
  int time_sleep = rand() % 4; // %20 b/c I don't want to wait forever
  sleep(time_sleep);

  release_pid(pid);
  pthread_exit(0);
}
/** Creates and initializes a data structure
  * Allocates a size of MAX_PID because PID's can
  *   have PID MIN_PID - MAX_PID
  *
  * @return = -1 if unsuccessful, 1 if successful
  */
int allocate_map(void) {
  bm = newBITMAP(MAX_PID);
  if(bm == 0) // If points to NULL
    return -1;
  else
    return 1;
}
/** Allocates and returns a pid
  *
  * @return = 1 if unable to allocate
  *           else returns pid
  */
int allocate_pid(void) {
  int pid = 1;
  for(int i = MIN_PID; i < MAX_PID; i++) {
    //printf("Getting bit %d\n",i);
    if(getBITMAP(bm,i) == 0) {
      setBITMAP(bm,i);
      pid = i;
      break;
    }
  }
  return pid;
}
/** Releases a pid
  *
  * @param pid = pid id
  */
void release_pid(int pid) {
  clearBITMAP(bm,pid);
}

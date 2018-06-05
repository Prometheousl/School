/*
 *  statistics.c
 *
 *  Created on: Feb 17,2018
 *  Author: Alex Lay
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include "int.h"
#include "da.h"

#define NUM_THREADS 3
float average;
int minimum;
int maximum;

DA *parseInput(int argc,char **argv);
void createThreads(pthread_t *, DA *integers);
void joinThreads(pthread_t *);
void *calcAverage(void *integers);
void *calcMinimum(void *integers);
void *calcMaximum(void *integers);

/** Write a multithreaded program that calculates various statistical values
  * for a list of numbers. This program will be passed a series of numbers on
  * the command line and will then create three separate worker threads.
  * One thread will determine the average of the numbers, the second
  * will determine the maximum value, and the third will determine the
  * minimum value. For example, suppose your program is passed the integers
  *
  * 90 81 78 95 79 72 85
  *
  * The program will report:
  *
  * The average value is 82
  * The minimum value is 72
  * The maximum value is 95
  *
  * The variables representing the average, minimum, and maximum values
  * will be stored globally. The worker threads will set these values, and the
  * parent thread will output the values once the workers have exited. (We
  * could obviously expand this program by creating additional threads
  * that determine other statistical values, such as median and standard
  * deviation.)
  */
int main(int argc,char **argv) {
  if(argc <= 1) {
    printf("Not enough arguments!\n");
    exit(0);
  }

  DA *integers = parseInput(argc,argv);
  pthread_t tid[NUM_THREADS];

  createThreads(tid,integers);
  joinThreads(tid);

  printf("The average value is %d\n",(int)average);
  printf("The minimum value is %d\n",minimum);
  printf("The maximum value is %d\n",maximum);
  return 0;
}
/** Reads the command line inputs in the form:
  *   integer integer integer etc...
  * Places integers in a dynamic array
  *
  * @param argc = The amount of command line arguments
  * @param argv = The command line arguments
  */
DA *parseInput(int argc,char **argv) {
  DA *integers = newDA(displayINT);
  for(int x = 1; x < argc; x++) {
      int number = atoi(argv[x]);
      insertDA(integers,newINT(number));
  }
  return integers;
}
/** Calculates the average of a dynamic array
  *   filled with integers.
  * Passed to a thread.
  *
  * @param integers = A dynamic array (DA)
  */
void *calcAverage(void *integers) {
  int total = 0;
  for(int x = 0; x < sizeDA(integers); x++) {
    total += getINT(getDA(integers,x));
  }
  average = total/sizeDA(integers);
  pthread_exit(0);
}
/** Calculates the minimum of a dynamic array
  *   filled with integers.
  * Passed to a thread.
  *
  * @param integers = A dynamic array (DA)
  */
void *calcMinimum(void *integers) {
  minimum = getINT(getDA(integers,0));
  for(int x = 1; x < sizeDA(integers); x++) {
    int temp = getINT(getDA(integers,x));
    if(temp < minimum)
      minimum = temp;
  }
  pthread_exit(0);
}
/** Calculates the maximum of a dynamic array
  *   filled with integers.
  * Passed to a thread.
  *
  * @param integers = A dynamic array (DA)
  */
void *calcMaximum(void *integers) {
  maximum = getINT(getDA(integers,0));
  for(int x = 1; x < sizeDA(integers); x++) {
    int temp = getINT(getDA(integers,x));
    if(temp > maximum)
      maximum = temp;
  }
  pthread_exit(0);
}
/** Creates NUM_THREADS amount of threads.
  *
  * Each thread requests a pid from the bitmap
  * The pid is then passed to the thread in a runner function.
  *
  * @param tid = An array of tid's of size NUM_THREADS
  */
void createThreads(pthread_t *tid, DA *integers) {
  // Average thread
  pthread_create(&tid[0],NULL,calcAverage,integers);
  // Minimum thread
  pthread_create(&tid[1],NULL,calcMinimum,integers);
  // Maximum thread
  pthread_create(&tid[2],NULL,calcMaximum,integers);
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

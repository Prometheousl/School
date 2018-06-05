/*
 *  producer_consumer.c
 *
 *  Created on: Feb 25,2018
 *  Author: Alex Lay
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include "buffer.h"
#include "int.h"

// Static functions
static void createProducerThreads(pthread_t *,int);
static void createConsumerThreads(pthread_t *,int);
static void *producerFunction(void *);
static void *consumerFunction(void *);

/** The main() function will initialize the buffer and create the separate
  * producer and consumer threads. Once it has created the producer and
  * consumer threads, the main() function will sleep for a period of time and,
  * upon awakening, will terminate the application. The main() function will be
  * passed three parameters on the command line:
  *     1. How long to sleep before terminating
  *     2. The number of producer threads
  *     3. The number of consumer threads
  */
int main(int argc,char **argv) {
  srand(time(NULL));
  int time_sleep;
  int num_producer_threads;
  int num_consumer_threads;
  /* 1. Get command line arguments argv[1],argv[2],argv[3] */
  if(argc < 3) {
    printf("Not enough arguments!\n");
    exit(0);
  }
  else {
    time_sleep = atoi(argv[1]);
    num_producer_threads = atoi(argv[2]);
    num_consumer_threads = atoi(argv[3]);
    printf("Time_sleep: %d\n",time_sleep);
    printf("Num_producer_threads: %d\n",num_producer_threads);
    printf("Num_consumer_threads: %d\n",num_consumer_threads);
  }
  /* 2. Initialize buffer */
  initializeBuffer();
  /* 3. Create producer thread(s) */
  pthread_t p_tid[num_producer_threads];
  createProducerThreads(p_tid,num_producer_threads);
  /* 4. Create consumer thread(s) */
  pthread_t c_tid[num_consumer_threads];
  createConsumerThreads(c_tid,num_consumer_threads);
  /* 5. Sleep */
  sleep(time_sleep);
  /* 6. Exit */
  printf("Final buffer is: ");
  printBuffer();
  exit(0);
}
static void createProducerThreads(pthread_t *p_tid, int num) {
  for(int x = 0; x < num; x++) {
    pthread_create(&p_tid[x],NULL,producerFunction,&p_tid[x]);
  }
}
static void createConsumerThreads(pthread_t *c_tid, int num) {
  for(int x = 0; x < num; x++) {
    pthread_create(&c_tid[x],NULL,consumerFunction,&c_tid[x]);
  }
}
static void *producerFunction(void *p_tid) {
  //long long tid = (long)p_tid;
  pthread_t tid = pthread_self();
  int alternator = 0;
  int time_sleep;
  buffer_item random_number;
  while(1) {
    if(alternator == 0) {
      time_sleep = rand() % 6;
      printf("Producer thread %lu sleeping for %d seconds.\n",tid,time_sleep);
      sleep(time_sleep);
      alternator = 1;
    }
    else {
      random_number = rand() % 100;
      int successful = insertItem(random_number);
      if(successful) {
        printf("TID %lu producing item %d\n",tid,random_number);
        printf("Buffer is now: ");
        printBuffer();
      }
      else
        printf("Couldn't produce item\n");
      alternator = 0;
    }
  }
  return NULL;
}
static void *consumerFunction(void *c_tid) {
  //int tid = getINT(c_tid);
  pthread_t tid = pthread_self();
  int alternator = 0;
  int time_sleep;
  buffer_item removed_item;
  while(1) {
    if(alternator == 0) {
      time_sleep = rand() % 6;
      printf("Consumer thread %lu sleeping for %d seconds.\n",tid,time_sleep);
      sleep(time_sleep);
      alternator = 1;
    }
    else {
      int successful = removeItem(&removed_item);
      if(successful) {
        printf("TID %lu removed item %d\n",tid,removed_item);
        printf("Buffer is now: ");
        printBuffer();
      }
      else
        printf("Couldn't remove item\n");
      alternator = 0;
    }
  }
  return NULL;
}

/*
 * buffer.c
 *
 *  Created on: Feb 25, 2018
 *      Author: Alex Lay
 *
 *  Desc: This class implements a buffer based on a
 *          circular queue data structure. It uses
 *          a mutex lock and semaphores to protect
 *          critical sections of data.
 */
#include "buffer.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

// Buffer stuff
buffer_item buffer[BUFFER_SIZE];
int size = BUFFER_SIZE;
int front;
int back;
// Thread synchronization stuff
pthread_mutex_t mutex;
sem_t empty,full;
// Static functions
static int insert(buffer_item item);
static int removeFromBuffer(buffer_item *item);
/** Initializes the mutual-exclusion object mutex,
  *   the empty & full semaphores, and the buffer indices.
  */
extern void initializeBuffer() {
  printf("Initializing buffer. Buffer size is %d.\n",size);
  front = back = -1;
  pthread_mutex_init(&mutex,NULL);
  sem_init(&empty,0,size);
  sem_init(&full,0,0);
}
/** Inserts an item into the buffer. Uses
  *   a mutex and two semaphores to ensure there will be
  *   no race conditions. (Producer code)
  *
  * @param item = An int
  * @return   0 if successful
  *          -1 if unsuccessful
  */
extern int insertItem(buffer_item item) {
  sem_wait(&empty);
  pthread_mutex_lock(&mutex);  // acquire
  /* critical section */
  int successful = insert(item);
  /* end critical section */
  pthread_mutex_unlock(&mutex);  // release
  sem_post(&full);
  if(successful)
    return 0;
  else // unsuccessful
    return -1;
}
/** Actually inserts an item into the buffer.
  * Mostly here for code readability
  * This is the critical section of the producer
  *
  * @param item = an int
  * @return   0 if successful
  *          -1 if unsuccessful
  */
static int insert(buffer_item item) {
  if((back == size - 1 && front == 0) ||
      (back == front - 1)) {
    printf("Can't insert %d. Queue is full.\n",item);
    return -1; // unsuccessful
  }
  else { // not full
    if(front == -1) {// Insert first element
      front = back = 0;
      buffer[back] = item;
    }
    else if(back == size - 1 && front != 0) {
      back = 0;
      buffer[back] = item;
    }
    else {
      back++;
      buffer[back] =  item;
    }
    return 0; // successful
  }
}
/** Removes an item from the buffer placing it in item.
  *
  * @param item = An int*
  * @return   0 if successful
  *          -1 if unsuccessful
  */
extern int removeItem(buffer_item *item) {
  sem_wait(&full);
  pthread_mutex_lock(&mutex);  // acquire
  /* critical section */
  int successful = removeFromBuffer(item);
  /* end critical section */
  pthread_mutex_unlock(&mutex);  // release
  sem_post(&empty);
  if(successful)
    return 0;
  else // unsuccessful
    return -1;
}
/** Actually removes an item from the buffer.
  * Mostly here for code readability
  * This is the critical section of the consumer
  *
  * @param item = an int*
  * @return   0 if successful
  *          -1 if unsuccessful
  */
static int removeFromBuffer(buffer_item *item) {
  if(front == -1) {// Queue is empty
    printf("Can't remove. Queue is empty.\n");
    return -1; // unsuccessful
  }
  else {
    *item = buffer[front];
    buffer[front] = -1;
    if(front == back) {
      front = -1;
      back = -1;
    }
    else if(front == size - 1)
      front = 0;
    else
      front++;
    return 0;
  }
}
/** print function for the buffer
  *  for debugging purposes
  */
extern void printBuffer() {
  printf("[");
  for(int x = 0; x < size; x++) {
    if(x != size - 1) {
      if(buffer[x] == -1)
        printf("NULL,");
      else
        printf("%d,",buffer[x]);
    }
    else {
      if(buffer[x] == -1)
        printf("NULL");
      else
        printf("%d",buffer[x]);
    }
  }
  printf("]\n");
}

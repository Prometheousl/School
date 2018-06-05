/*
 *  statistics.c
 *
 *  Created on: Feb 6,2018
 *  Author: Alex Lay
 *
 *  Desc: Implements an API for a PID manager.
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "bitmap.h"

#define MIN_PID 300
#define MAX_PID 5000

#define NUM_THREADS 20

BITMAP *bm;

/** Creates and initializes a data structure
  * Allocates a size of MAX_PID - MIN_PID because PID's can
  *   have PID MIN_PID to MAX_PID
  *
  * @return = -1 if unsuccessful, 1 if successful
  */
extern int allocate_map(void) {
  int bitmap_size = MAX_PID - MIN_PID;
  bm = newBITMAP(bitmap_size);
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
extern int allocate_pid(void) {
  int pid = 1;
  for(int i = 0; i < MAX_PID - MIN_PID; i++) {
    //printf("Getting bit %d\n",i);
    if(getBITMAP(bm,i) == 0) {
      setBITMAP(bm,i);
      pid = i + MIN_PID;
      break;
    }
  }
  return pid;
}
/** Releases a pid
  *
  * @param pid = pid id
  */
extern void release_pid(int pid) {
  clearBITMAP(bm,pid - MIN_PID);
}

// Used for testing
int main() {
  allocate_map();

  for(int x = 0; x < 10; x++) {
    int pid = allocate_pid();
    printf("Allocated pid %d\n",pid);
  }
  for(int x = 0; x < 20; x++) {
    int value = getBITMAP(bm,x);
    printf("Value at index %d is: %d\n",x + MIN_PID,value);
  }
  release_pid(10);
  release_pid(11);
  printBITMAP(stdout,bm);
  return 0;
}

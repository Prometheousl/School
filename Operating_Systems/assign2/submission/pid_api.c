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

// Helper functions so I can debug...
extern void print_underlying_bitmap() {
  printBITMAP(stdout,bm);
}

extern int get_pid_value(int pid) {
  return getBITMAP(bm,pid - MIN_PID);
}

extern void print_pids() {
  for(int x = 0; x < 20; x++) {
    int pid = x + MIN_PID;
    printf("PID[%d] = %d\n",pid,get_pid_value(pid));
  }
}

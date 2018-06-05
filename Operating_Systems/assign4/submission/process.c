/*
 * STRING.c
 *
 *  Created on: Sep 6, 2017
 *  Author: Alex Lay
 */

#include <stdio.h>
#include <stdlib.h>
#include "process.h"

struct process {
    int arrival_time;
    int priority;
    int processor_time;
    int remaining_time;
    enum STATE state;
    int PID;
};

/** Creates a new PROCESS object
  *
  * @param x = The string to cast to STRING
  * @return = the STRING object
  */
PROCESS *
newPROCESS(int a_time,int priority, int p_time) {
    PROCESS *p = malloc(sizeof(PROCESS));
    if (p == 0) {
    	fprintf(stderr,"out of memory\n"); exit(1);
    }
    p->arrival_time = a_time;
    p->priority = priority;
    p->processor_time = p_time;
    p->remaining_time = p_time;
    p->state = WAITING;
    p->PID = -1;
    return p;
}
PROCESS *
getPROCESS(PROCESS *p) {
  return p;
}
/** Returns the arrival time
  *
  * @param p = the PROCESS object
  * @return = the arrival_time
  */
int
getArrivalTime(PROCESS *p) {
    return p->arrival_time;
}
/** Returns the priority
  *
  * @param p = the PROCESS object
  * @return = the priority
  */
int
getPriority(PROCESS *p) {
    return p->priority;
}
/** Returns the processor time
  *
  * @param p = the PROCESS object
  * @return = the processor_time
  */
int
getProcessorTime(PROCESS *p) {
    return p->processor_time;
}
/** Returns the remaining time
  *
  * @param p = the PROCESS object
  * @return = the remaining_time
  */
int
getRemainingTime(PROCESS *p) {
    return p->remaining_time;
}
/** Decrements the remaining time
  */
void
decrementRemainingTime(PROCESS *p) {
  p->remaining_time -= 1;
}
void
lowerPriority(PROCESS *p) {
  p->priority += 1;
}
/** Returns the PID of the process
  */
int
getPID(PROCESS *p) {
  return p->PID;
}
/** sets the PID
  */
void
setPID(PROCESS *p, int pid) {
  p->PID = pid;
}
void
setState(PROCESS *p, enum STATE s) {
  p->state = s;
}
enum STATE
getState(PROCESS *p) {
  return p->state;
}
/** Displays the PROCESS object
  *
  * @param fp = the file to display to
  * @param p = The PROCESS object
  */
void
displayPROCESS(FILE *fp,void *p) {
    fprintf(fp,"[%d, %d, %d, %d]",getArrivalTime(p),
      getPriority(p),getProcessorTime(p),getRemainingTime(p));
}
/** Compares two Processes
  *
  * @param x,y = PROCESS objects
  * @return = Result of comparison
  */
int
comparePROCESS(void *x, void *y) {
  int arrival_time1 = getArrivalTime(x);
  int arrival_time2 = getArrivalTime(y);
  if(arrival_time1 > arrival_time2)
    return 1;
  else if(arrival_time1 < arrival_time2)
    return -1;
  else // equal
    return 0;
}
/** Frees the process object
  *
  * @param p =- the PROCESS object to free
  */
void
freePROCESS(PROCESS *p) {
	free(p);
}

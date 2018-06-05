/*
 * STRING.h
 *
 *  Created on: Sep 6, 2017
 *      Author: alexl
 */

#ifndef STRING_H_
#define STRING_H_

#include <stdio.h>

enum STATE{SUSPENDED,TERMINATED,RUNNING,WAITING};

typedef struct process PROCESS;

extern PROCESS *newPROCESS(int,int,int);
extern PROCESS *getPROCESS(PROCESS *);
extern int getArrivalTime(PROCESS *);
extern int getPriority(PROCESS *);
void lowerPriority(PROCESS *);
extern int getProcessorTime(PROCESS *);
extern int getRemainingTime(PROCESS *);
extern void decrementRemainingTime(PROCESS *);
extern int getPID(PROCESS *);
extern void setPID(PROCESS *, int);
void setState(PROCESS *, enum STATE);
enum STATE getState(PROCESS *);
extern void displayPROCESS(FILE *,void *);
extern int comparePROCESS(void *, void *);
extern void freePROCESS(PROCESS *);

#endif /* STRING_H_ */

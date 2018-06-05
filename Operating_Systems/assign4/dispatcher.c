/*
 *  dispatcher.c
 *
 *  Created on: March 22,2018
 *  Author: Alex Lay
 */
 #define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>
#include "scanner.h"
#include "process.h"
#include "rbt.h"
#include "queue.h"

// Global variables
int _ticks = 0;
QUEUE *system_q;
QUEUE *priority_q1;
QUEUE *priority_q2;
QUEUE *priority_q3;
//RBT *rbt;

// Static functions
static RBT *readProcessList(char *);
static PROCESS *readProcess(FILE *);
static void dispatchProcesses(RBT *rbt);
static void enqueueProcess(PROCESS *p);
static void runProcesses();
static PROCESS *getUserProcess();
static void terminateProcess(PROCESS *p);
static void suspendProcess(PROCESS *p);
static pid_t startProcess(PROCESS *p);
static void restartProcess(PROCESS *p);
static int notEmpty(RBT *rbt);
static void startSystemProcess(PROCESS *temp);
static void startUserProcess(PROCESS *temp);
static void restartUserProcess(PROCESS *temp);

// Debugging functions
//static void displayQueues();

/** Simulates a dispatcher system
  */
int main(int argc,char **argv) {
  char *process_list;
  /* 1. Get command line arguments argv[1] */
  if(argc < 2) {
    printf("Not enough arguments!\n");
    exit(0);
  }
  else {
    process_list = argv[1];
    //printf("process_list is: %s\n",process_list);
  }
  /* 2. Read processes from list & store in data structure(s) */
  RBT *rbt = readProcessList(process_list);
  //displayRBT(stdout,rbt);
  /* 3. Dispatch the processes to their appropriate queues when
          it is their arrival time */
  //createThread(rbt);
  dispatchProcesses(rbt);
  exit(0);
}
/** Reads the process list specified into a red-black tree
  *   so searching for a process will take O(logn)
  * Processes are stored in the PROCESS struct
  *
  * @param process_list = the name of the file to read from
  * @return = the RBT containing all the processes
  */
static RBT *readProcessList(char *process_list) {
  //printf("In readProcessList!\n");
  RBT *rbt = newRBT(displayPROCESS,comparePROCESS);
  FILE *file = fopen(process_list,"r");
  PROCESS *p = readProcess(file);
  while(!feof(file)) {
    if(p != NULL) {
      insertRBT(rbt,p);
    }
    p = readProcess(file);
  }
  int successfulClose = fclose(file);
  if(successfulClose != 0)
    printf("Couldn't close file!\n");
  return rbt;
}

static PROCESS *readProcess(FILE *file) {
  //printf("In readProcess.\n");
  char *line = readLine(file);
  //printf("Line is %s\n",line);
  if(line != NULL) {
    int arrival_time = line[0] - '0';
    int priority = line[3] - '0';
    int processor_time = line[6] - '0';
    return newPROCESS(arrival_time,priority,processor_time);
  }
  else {
    return NULL;
  }
}

/* 3. Loop:
        if(process's arrival_time == current tick time)
            place on appropriate queue (based on priority)

        Run system jobs, otherwise run lower queues

        Sleep(1 second)

        if(user task's processor time used up)
          terminate it
        else
          suspend it

        Rotate tasks (Round Robin)

        _ticks++;
      End Loop */
static void dispatchProcesses(RBT *rbt) {
  system_q = newQUEUE(displayPROCESS);
  priority_q1 = newQUEUE(displayPROCESS);
  priority_q2 = newQUEUE(displayPROCESS);
  priority_q3 = newQUEUE(displayPROCESS);
  PROCESS *temp;
  while(sizeRBT(rbt) != 0) { // while still processes in list
    temp = newPROCESS(_ticks,0,0);
    /* Enqueue every process at _ticks into an appropriate queue */
    while(findRBT(rbt,temp) != 0) { // while process at _ticks
      temp = getPROCESS(getRBT(rbt,temp));
      //printf("Found one for tick %d. Process is \n",_ticks);
      //displayPROCESS(stdout,temp);
      deleteRBT(rbt,temp);
      //printf("\nnew tree is:\n");
      //displayRBT(stdout,rbt);
      /* Add process to appropriate queue */
      enqueueProcess(temp);
    }
    //displayQueues();
    runProcesses();
    sleep(1);

    //rotateTasks();
    _ticks++;
  }
  while(notEmpty(rbt)){
    runProcesses();
    sleep(1);
  }
}
/** enqueues a process based on its priority
  *   priority 0 goes to system_q
  *   priority 1 goes to priority_q1
  *   priority 2 goes to priority_q2
  *   priority 3 goes to priority_q3
  *
  * @param p = The PROCESS to enqueue
  */
static void enqueueProcess(PROCESS *p) {
  int priority = getPriority(p);
  switch(priority) {
    case 0:
      enqueue(system_q,p);
      break;
    case 1:
      enqueue(priority_q1,p);
      break;
    case 2:
      enqueue(priority_q2,p);
      break;
    case 3:
      enqueue(priority_q3,p);
      break;
  }
}
static void runProcesses() {
    //printf("In runProcesses()!\n");
    PROCESS *temp;
    if(sizeQUEUE(system_q) != 0) {
      // run system process to completion preempting any other jobs currently running
      temp = dequeue(system_q);
      //printf("Found a system process: ");
      //displayPROCESS(stdout,temp);
      startSystemProcess(temp);
    }
    else { // run user process
      temp = getUserProcess();
      //printf("After getUserProcess()\n");
      if(temp != NULL) {
        //printf("Found a user process: ");
        //displayPROCESS(stdout,temp);
        if(getState(temp) != SUSPENDED)
          startUserProcess(temp);
        else {
          //printf("Should restart user process\n");
          restartUserProcess(temp);
        }
      }
    }
  //}
  //exit(0);
}
static void startSystemProcess(PROCESS *temp) {
  pid_t child_pid = startProcess(temp);
  if(child_pid != 0) { // parent process
    sleep(getProcessorTime(temp));
    terminateProcess(temp);
  }
}
static void startUserProcess(PROCESS *temp) {
  pid_t child_pid = startProcess(temp);
  if(child_pid != 0) { // parent process
    sleep(1);
    decrementRemainingTime(temp);
    if(getRemainingTime(temp) == 0)
      terminateProcess(temp);
    else
      suspendProcess(temp);
  }
}
static void restartUserProcess(PROCESS *temp) {
  restartProcess(temp);
  sleep(1);
  decrementRemainingTime(temp);
  if(getRemainingTime(temp) == 0)
    terminateProcess(temp);
  else
    suspendProcess(temp);
}
static int notEmpty(RBT *rbt) {
  int notEmpty = 1;
  if(  sizeRBT(rbt) == 0
    && sizeQUEUE(system_q) == 0
    && sizeQUEUE(priority_q1) == 0
    && sizeQUEUE(priority_q2) == 0
    && sizeQUEUE(priority_q3) == 0) {
      notEmpty = 0;
    }
    return notEmpty;
}
static PROCESS *getUserProcess() {
  //printf("In getUserProcess()!\n");
  //displayQueues();
  PROCESS *user_process = NULL;
  if(sizeQUEUE(priority_q1) != 0)
    user_process = dequeue(priority_q1);
  else if(sizeQUEUE(priority_q2) != 0)
    user_process = dequeue(priority_q2);
  else if(sizeQUEUE(priority_q3) != 0)
    user_process = dequeue(priority_q3);
  return user_process;
}
static pid_t startProcess(PROCESS *p) {
  pid_t child_pid = fork();
  if(child_pid == 0) { // child process
    char *argv[] = {"process",NULL};
    setState(p,RUNNING);
    execvp("./process",argv);
    exit(0);
  }
  else {
    setPID(p,(int)child_pid);
  }
  return child_pid;
}
static void terminateProcess(PROCESS *p) {
  kill(getPID(p),SIGINT);
  setState(p,TERMINATED);
  int status;
  waitpid(getPID(p), &status, WUNTRACED);
}
static void suspendProcess(PROCESS *p) {
  kill(getPID(p),SIGTSTP);
  setState(p,SUSPENDED);
  if(getPriority(p) != 3)
    lowerPriority(p);
  enqueueProcess(p);
  int status;
  waitpid(getPID(p), &status, WUNTRACED);
}
static void restartProcess(PROCESS *p) {
  kill(getPID(p),SIGCONT);
}
/** Debugging function to print all the
  *   ready queues
  */
/*static void displayQueues() {
  printf("\nSystem queue: ");
  displayQUEUE(stdout,system_q);
  printf("\nPriority queue 1: ");
  displayQUEUE(stdout,priority_q1);
  printf("\nPriority queue 2: ");
  displayQUEUE(stdout,priority_q2);
  printf("\nPriority queue 3: ");
  displayQUEUE(stdout,priority_q3);
  printf("\n");
}*/

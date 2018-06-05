/*
 * Collatz_conjecture.c
 *
 * Created on: Feb 5, 2018
 * Author: Alex Lay
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/** Takes a positive integer and implements the
  *   Collatz conjecture for a positive integer N:
  *
  * if N is even: N = N/2
  * if N is odd:  N = 3N + 1
  *
  * N should eventually equal 1.
  *
  * Implements the algorithm by forking a child process
  */
int main(int argc,char **argv) {
  pid_t child_pid, wpid;
  int status = 0;

  // Parent process
  if(argc < 2) {
    printf("Invalid. Must enter initial integer!\n");
    return 0;
  }

  int value = atoi(argv[1]); // handles float values

  if(value < 0) {
    printf("Invalid. Must enter a positive intiger!\n");
    return 0;
  }

  // Child Process
  if((child_pid = fork()) == 0) {
    printf("%d,",value);
    while(value != 1) {
      // do calculation
      if(value % 2 == 0)
        value /= 2;
      else if(value % 2 == 1)
        value = value * 3 + 1;
      // print value
      if(value != 1)
        printf("%d,",value);
      else
        printf("%d\n",value);
    }
    exit(0);
  }


  while((wpid = wait(&status)) > 0);
  return 0;
}

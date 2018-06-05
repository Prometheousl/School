#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
  pid_t pid, pid1;

  printf("Parent is initially: %d\n",getpid());

  /* fork a child process */
  pid = fork();
  printf("Child is %d\n",pid);
  if (pid < 0) { /* error occurred */
    fprintf(stderr, "Fork Failed");
    return 1;
  }
  else if (pid == 0) { /* child process */
    pid1 = getpid();
    printf("A - child: pid = %d\n",pid); /* A */
    printf("B - child: pid1 = %d\n",pid1); /* B */
  }
  else { /* parent process */
    pid1 = getpid();
    printf("C - parent: pid = %d\n",pid); /* C */
    printf("D - parent: pid1 = %d\n",pid1); /* D */
    wait(NULL);
  }
  return 0;
}

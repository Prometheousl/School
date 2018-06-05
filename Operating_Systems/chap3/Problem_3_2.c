#include <stdio.h>
#include <unistd.h>
int main() {
  /* fork a child process */
  fork();
  /* fork another child process */
  fork();
  /* and fork another */
  fork();
  printf("I'm a process!");
  return 0;
}

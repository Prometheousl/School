#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

static char *itoa(int x);

int main(void) {
  int time_quant = 1;
  char *argument = itoa(time_quant);
  printf("Argument is %s\n",argument);
  const char *argv[] = {argument};
  printf("Argv[0] is %s\n",argv[0]);
  return 0;
}

static char *itoa(int x) {
  char *buffer = malloc(sizeof(char) * sizeof(int) * 4 + 1);
  if(buffer) {
    sprintf(buffer,"%d",x);
  }
  return buffer;
}

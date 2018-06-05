/*
 *  shell_history.c
 *
 *  Created on: Feb 5, 2018
 *  Author: Alex Lay
 *
 *  Usage: Implements a pseudo-shell.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "stack.h"
#include "my_string.h"
#include "str_array.h"

#define MAX_LINE 80 /* The maximum length command */
int ampersand_included = 0;

static void skipWhiteSpace(FILE *fp);
void *allocateMsg(size_t size,char *where);
static void *reallocateMsg(void *s,size_t size,char *where);
char *readToken(FILE *fp);
char *readLine(FILE *fp);

void readUserInput(char *, char *args[]);
void forkChildProcess(char *args[]);
void displayRecentCommands(STACK *p_stack);
int isExclamation(char *args[]);
int isValidInput(char *);
int getRestOfStringNumbers(char *args[]);
void STR_ARRAYtoCommand(STR_ARRAY *arr,char * args[]);

/** Impements a simplified version of a shell.
  *
  * Takes command line arguments and forks them.
  * If ampersand is at the end, runs in the background.
  *
  * ---------------Commands----------------------
  * "history" command shows last 10 processes run.
  * "!!" command forks the most recent process run.
  * "!N" command forks the process at N in history,
  *     where N is an integer.
  */
int main(void) {
  char *args[MAX_LINE / 2 + 1]; /* command line arguments */
  int should_run = 1; /* flag to determine when to exit program */

  // Process Stack
  STACK *p_stack = newSTACK(displaySTR_ARRAY);

  while (should_run) {
    printf("osh>");
    fflush(stdout);

    // Read the arguments
    char *arguments_string = readLine(stdin);

    if(isValidInput(arguments_string)) {
      // Reads user input into separate tokens (in args)
      readUserInput(arguments_string,args);

      if(!isExclamation(args)) // Push onto process stack
        push(p_stack,newSTR_ARRAY(args));

      if(strcmp(*args,"exit") == 0) // Check if exit
        should_run = 0;
      else if(strcmp(*args,"history") == 0)
        displayRecentCommands(p_stack);
      else if(strcmp(*args,"!!") == 0) {
        if(sizeSTACK(p_stack) == 0) {
          printf("No commands in history.\n");
        }
        else { // Fork most recent command
          STR_ARRAY *arr = pop(p_stack);
          STR_ARRAYtoCommand(arr,args);
          forkChildProcess(args);
          // Push back on stack
          push(p_stack,arr);
        }
      }
      else if(*args[0] == '!') { // Fork specified command
        int num = getRestOfStringNumbers(args);
        if(num < 1 || num > sizeSTACK(p_stack)) {
          printf("No such command in history.\n");
        }
        else {
          // Get specified process
          DA *processes = getUnderlyingDA(p_stack);
          // Args = process at num - 1
          STR_ARRAY *process = getDA(processes,num - 1);
          STR_ARRAYtoCommand(process,args);
          forkChildProcess(args);
          push(p_stack,process);
        }
      }
      else
        forkChildProcess(args);
    }
  }

  return 0;
}
/** Appends the STR_ARRAY to args.
  *
  * @param arr = The STR_ARRAY to append
  * @param args[] = The 2d char array to append to
  */
void STR_ARRAYtoCommand(STR_ARRAY *arr,char * args[]) {
  DA *da = getSTR_ARRAY(arr);
  printf("Size of DA is %d\n",sizeDA(da));
  for(int x = 0; x < sizeDA(da); x++) {//
    args[x] = getSTRING(getDA(da,x));
  }
}
/** Returns the integers following the First
  *   character in a string.
  * If a non-integer is found, returns -1.
  *
  * @param args = The 2d char array to parse
  * @return = The integers following the
  *   first character
  */
int getRestOfStringNumbers(char *args[]) {
  char string_num[41] = "";
  unsigned int length = strlen(*args);
  for(unsigned int x = 1; x < length; x++) {
    if(isdigit(args[0][x])) {
      char str[2] = "\0";
      str[0] = args[0][x];
      strcat(string_num,str);
    }
    else
      return -1;
  }
  int num = atoi(string_num);
  return num;
}
/** Checks if user input is valid.
  *
  * @param input = user input
  * @return = 1 if valid
  *           0 if invalid
  */
int isValidInput(char *input) {
  if(input == NULL)
    return 0;
  else if(strcmp(input,"") == 0)
    return 0;
  return 1;
}
/** Checks if args contains an
  *   exclamation mark.
  *
  * @param args = 2d char array to parse
  * @return = 1 if contains exclamation
  *           0 if doesn't contain
  */
int isExclamation(char* args[]) {
  if(strcmp(*args,"!!") == 0)
    return 1;
  else if(*args[0] == '!')
    return 1;

  return 0;
}
/** Displays up to the ten most recent
  *   commands in the stack.
  *
  * @param p_stack = the stack w/commands
  */
void displayRecentCommands(STACK *p_stack) {
  int num_processes = 0;
  STACK *temp_stack = newSTACK(displaySTR_ARRAY);
  int initial_size = sizeSTACK(p_stack);
  if(initial_size >= 10)
    num_processes = 10;
  else
    num_processes = initial_size;

  // Display num_process of processes
  for(int p = 0; p < num_processes; p++) {
    STR_ARRAY *temp = pop(p_stack);
    printf("%d ",sizeSTACK(p_stack) + 1);
    displaySTR_ARRAY(stdout,temp);
    printf("\n");
    push(temp_stack,temp);
  }

  int initial_temp_size = sizeSTACK(temp_stack);

  // Push back processes that were popped
  for(int p = 0; p < initial_temp_size; p++)
    push(p_stack,pop(temp_stack));
}
/** Forks a child process specified in args.
  * If no ampersand, parent process waits
  *   for child process to execute.
  *
  * @param args = The process to fork
  */
void forkChildProcess(char *args[]) {
  pid_t child_pid;
  int status;
  if((child_pid = fork()) == 0) // child forked successfully
    execvp(*args,args);
  else if(!ampersand_included)
    while(wait(&status) != child_pid);
}
/** Reads the user input from arguments_string
  *   and puts it in args.
  *
  * @param arguments_string = the string to
  *   read arguments from
  * @param args = the 2d char array to put
  *   the arguments in
  */
void readUserInput(char *arguments_string,char *args[]) {
  ampersand_included = 0;
  int arg_index = 0;

  args[arg_index] = strtok(arguments_string, " \t");

  while(arguments_string && args[arg_index]) {
    arg_index++;
    args[arg_index] = strtok(NULL, " \t");
    if(args[arg_index] != NULL && *args[arg_index] == '&') {
      ampersand_included = 1;
    }
  }

  return;
}
/** Reads a line of input from the user
  *
  * @param fp = The file to read from
  */
char *readLine(FILE *fp) {
    int ch,index;
    char *buffer;
    int size = 512;

    ch = fgetc(fp);
    if (ch == EOF) return 0;

    buffer = allocateMsg(size,"readLine");

    index = 0;
    while (ch != '\n')
        {
        if (ch == EOF) break;
        if (index > size - 2)
            {
            ++size;
            buffer = reallocateMsg(buffer,size,"readLine");
            }
        buffer[index] = ch;
        ++index;
        ch = fgetc(fp);
        }


    if (index > 0)              //there is something in the buffer
        clearerr(fp);           //so force the read to be good

    buffer[index] = '\0';

    return buffer;
}
/** Reads a token of input from a file.
  *   where token = no whitespace
  *
  * @param fp = the file to read from
  */
char *readToken(FILE *fp) {
  int ch,index;
  char *buffer;
  int size = 80;

  skipWhiteSpace(fp);

  ch = fgetc(fp);
  if (ch == EOF) return 0;

  buffer = allocateMsg(size,"readToken");

  index = 0;
  while (!isspace(ch))
      {
      if (ch == EOF) break;
      if (index > size - 2)
          {
          ++size;
          buffer = reallocateMsg(buffer,size,"readToken");
          }
      buffer[index] = ch;
      ++index;
      ch = fgetc(fp);
      }

  /* push back the character that got us out of this loop */

  ungetc(ch,fp);

  if (index > 0)              //there is something in the buffer
      clearerr(fp);           //so force the read to be good

  buffer[index] = '\0';

  return buffer;
}

static void skipWhiteSpace(FILE *fp) {
    int ch;

    // Read chars until a non-whitespace character is encountered
    while ((ch = fgetc(fp)) != EOF && isspace(ch))
        continue;

    // Push non-space character back on
    if (ch != EOF) ungetc(ch,fp);
}
/** Allocates a string of size size.
  *
  * @param size = The size to allocate
  * @param where = where in memory to allocate
  */
void *allocateMsg(size_t size,char *where) {
    char *s;
    s = malloc(size);
    if (s == 0) {
        fprintf(stderr,"%s: could not allocate string, out of memory\n",
            where);
        exit(3);
    }

    return s;
}
/** Reallocates a string of size size
  *
  * @param s = The object to resize
  * @param size = The size to resize to
  * @param where = where in memory to reallocate to
  */
static void *reallocateMsg(void *s,size_t size,char *where) {
    char *t;
    t = realloc(s,size);
    if (t == 0) {
        fprintf(stderr,"%s: could not reallocate string, out of memory\n",
            where);
        exit(3);
    }

    return t;
}

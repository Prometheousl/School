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
void printCharArray(char *array[]);

void readUserInput(char *, char *args[]);
void forkChildProcess(char *args[]);
void displayRecentCommands(STACK *p_stack);
int isExclamation(char *args[]);
int isValidInput(char *);
int getRestOfStringNumbers(char *args[]);
void STR_ARRAYtoCommand(STR_ARRAY *arr,char * args[]);

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
      else if(*args[0] == '!') {
        printf("First character is an exclamation!\n");
        printf("Checking rest of string.\n");
        int num = getRestOfStringNumbers(args);
        printf("Num is %d\n",num);
        if(num < 1 || num > sizeSTACK(p_stack)) {
          printf("No such command in history.\n");
        }
        else if(num != -1) {
          // Get specified process
          DA *processes = getUnderlyingDA(p_stack);
          printf("Getting process\n");
          // Args = process at num
          STR_ARRAY *process = getDA(processes,num - 1);
          STR_ARRAYtoCommand(process,args);
          forkChildProcess(args);
          push(p_stack,process);
        }
      }
      else {
        /**
        * After reading user input, the steps are:
        * (1) fork a child process using fork()
        * (2) the child process will invoke execvp()
        * (3) if command included &, parent will NOT invoke wait()
        */
        //printf("Executing rest of program.\n");
        forkChildProcess(args);
      }
    }
  }

  return 0;
}

void STR_ARRAYtoCommand(STR_ARRAY *arr,char * args[]) {
  DA *da = getSTR_ARRAY(arr);
  displayDA(stdout,da);
  //char **temp = (char**)extractDA(da);
  //int x = 0;
  printf("Size of DA is %d\n",sizeDA(da));
  for(int x = 0; x < sizeDA(da); x++) {//
    args[x] = getSTRING(getDA(da,x));
    printf("args[%d] = %s\n",x,getSTRING(getDA(da,x)));
  }
}

int getRestOfStringNumbers(char *args[]) {
  printf("In getRestOfStringNumbers\n");
  printf("Args is %s\n",*args);
  printf("Length of args is %d\n",strlen(*args));
  char string_num[41] = "";
  for(unsigned int x = 1; x < strlen(*args); x++) {
    printf("In for loop. Character is %c\n",args[0][x]);
    if(isdigit(args[0][x])) {
      printf("It's a digit!\n");
      char str[2] = "\0";
      str[0] = args[0][x];
      printf("String is %s\n",str);
      printf("Adding %s to %s\n",str,string_num);
      strcat(string_num,str);
      printf("String num is now %s\n",string_num);
    }
    else {
      printf("Not a digit.\n");
      return -1;
    }
  }
  printf("Number is %s\n",string_num);
  int num = atoi(string_num);
  return num;
}

int isValidInput(char *input) {
  if(input == NULL)
    return 0;
  else if(strcmp(input,"") == 0)
    return 0;
  return 1;
}

int isExclamation(char* args[]) {
  if(strcmp(*args,"!!") == 0) {
    //printf("Characters are !!\n");
    return 1;
  }
  else if(*args[0] == '!') {
    //printf("First character of args is an exclamation!\n");
    //for(int x = 0; x < )
    return 1;
  }
  //printf("Not exclamation!\n");
  return 0;
}

void displayRecentCommands(STACK *p_stack) {
  //printf("In history\n");
  int num_processes = 0;
  STACK *temp_stack = newSTACK(displaySTR_ARRAY);
  int initial_size = sizeSTACK(p_stack);
  if(initial_size >= 10)
    num_processes = 10;
  else
    num_processes = initial_size;

  /*printf("Stack before for loop:\n");
  visualizeSTACK(stdout,p_stack);
  printf("\n");*/
  // Display num_process of processes
  for(int p = 0; p < num_processes; p++) {
    STR_ARRAY *temp = pop(p_stack);
    printf("%d ",sizeSTACK(p_stack) + 1);
    displaySTR_ARRAY(stdout,temp);
    printf("\n");
    //printf("%d %s\n",sizeSTACK(p_stack),process);
    push(temp_stack,temp);

    /*printf("Stack in loop:\n");
    visualizeSTACK(stdout,p_stack);
    printf("\n");*/
  }

  /*printf("Temp stack after the loop:\n");
  visualizeSTACK(stdout,temp_stack);
  printf("\n");
  printf("Size of temp stack is %d\n",sizeSTACK(temp_stack));

  printf("Stack after for loop:\n");
  visualizeSTACK(stdout,p_stack);
  printf("\n");*/

  int initial_temp_size = sizeSTACK(temp_stack);

  // Push back processes that were popped
  for(int p = 0; p < initial_temp_size; p++) {
    push(p_stack,pop(temp_stack));
    /*printf("Temp stack in the loop:\n");
    visualizeSTACK(stdout,temp_stack);
    printf("\n");*/
  }

  /*printf("Stack at end of history:\n");
  visualizeSTACK(stdout,p_stack);
  printf("\n");*/
}

void forkChildProcess(char *args[]) {
  //printf("In forkChildProcess\n");
  pid_t child_pid;
  int status;
  if((child_pid = fork()) == 0) { // child forked successfully
    //printf("Pid right before execvp is %d\n",getpid());
    execvp(*args,args);
  }
  else if(!ampersand_included) {
    //printf("Pid while waiting is %d\n",getpid());
    while(wait(&status) != child_pid);
  }
}

void printCharArray(char *arr[]) {
  int arg_index = 0;

  while(arr[arg_index]) {
    printf("array[%d]: %s\n",arg_index,arr[arg_index]);
    arg_index++;
  }
}

void readUserInput(char *arguments_string,char *args[]) {
  ampersand_included = 0;
  int arg_index = 0;

  //printf("String is: %s\n",arguments_string);

  args[arg_index] = strtok(arguments_string, " \t");
  printf("Args[0] is: %s\n",args[0]);
  //printf("String is: %s\n",arguments_string);

  while(arguments_string && args[arg_index]) {
    arg_index++;
    args[arg_index] = strtok(NULL, " \t");
    if(args[arg_index] != NULL && *args[arg_index] == '&') {
      ampersand_included = 1;
    }
    printf("Args[%d] is: %s\n",arg_index,args[arg_index]);
  }

  return;
}

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

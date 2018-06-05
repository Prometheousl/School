#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "decision_tree.h"
#include "input/scanner.h"
#include "casting/m_string.h"
#include "casting/int.h"
#include "casting/conversions.h"
#include "data_structures/stack.h"

static int isOperator(char *);
static int evaluateOperation(char *,int,int);
static int priority(char *op);
static void setValues(QUEUE *postfix_queue,DA *columns,DA *values);
static void setValue(QUEUE *postfix_queue,char *col_name,int value);

/** Converts the infix queue to the postfix queue
  *
  * @param infix_queue = The infix queue
  */
extern QUEUE *infixToPostfix(QUEUE *infix_queue) {
	STACK *operators = newSTACK(displaySTRING);
	QUEUE *postfix_queue = newQUEUE(displaySTRING);
  QUEUE *temp = copyQUEUE(infix_queue);
	STRING *token = dequeue(temp);
	STRING *semicolon = newSTRING(";");
	while(strcmp(getSTRING(token),getSTRING(semicolon)) != 0) {
    //printf("Infix Queue: ");
    //displayQUEUE(stdout,temp);
    //printf("\n");
    //printf("Postfix Queue: ");
    //displayQUEUE(stdout,postfix_queue);
    //printf("\n");
    //printf("Operators Stack: ");
    //displaySTACK(stdout,operators);
    //printf("\n");
		if((strcmp(getSTRING(token),"AND") != 0 && strcmp(getSTRING(token),"OR") != 0) &&
        (isalpha(*getSTRING(token)) || isNumber(getSTRING(token)))) {
			// Append variable/number to postfix queue
			enqueue(postfix_queue,token);
		}
		else { // token is an operator
			if(*getSTRING(token) == ')') { // Append all operators to postfix_queue until '(' found
				STRING *x = pop(operators);
				while(sizeSTACK(operators) != 0 && *getSTRING(x) != '(') {
					enqueue(postfix_queue,x);
					x = pop(operators);
				}
			}
			else if(sizeSTACK(operators) == 0 || *getSTRING(token) == '(' || *getSTRING(peekSTACK(operators)) == '('){
        //printf("Pushing %s to stack\n",getSTRING(token));
				push(operators,token);
			}
			else if(isOperator(getSTRING(token))){
        //printf("Found an operator.\n");
				while(sizeSTACK(operators) != 0 && priority(getSTRING(peekSTACK(operators))) >= priority(getSTRING(token))) {
          //printf("Is %s >= %s? Result is %d\n",getSTRING(peekSTACK(operators)),getSTRING(token),
                  //priority(getSTRING(peekSTACK(operators))) >= priority(getSTRING(token)));
					enqueue(postfix_queue,pop(operators));
				}
				push(operators,token);
			}
		}

		token = dequeue(temp);
	} // end while
	// Append the rest of the operators to the postfix_queue
	while(sizeSTACK(operators) != 0) {
		enqueue(postfix_queue,pop(operators));
	}
	return postfix_queue;
}
/**
 * Takes the postfix qeueue and evaluates it.
 *
 * It does this by pushing values to a stack
 * and evaluating operators when they appear.
 *
 * @param postfix_queue = A postfix queue
 */
extern int calculatePostfix(QUEUE *postfix_queue,DA *columns,DA *values) {
  QUEUE *q_temp = copyQUEUE(postfix_queue);
  setValues(q_temp,columns,values);
	int result = 0,op1 = 0,op2 = 0;
	STRING *value;
	INTEGER *temp;
	STACK *stack = newSTACK(displayINT);
	while(sizeQUEUE(q_temp) != 0) {
    /*printf("Stack is:\n");
    displaySTACK(stdout,stack);
    printf("\n");
    printf("Queue is:\n");
    displayQUEUE(stdout,q_temp);
    printf("\n");*/
		value = dequeue(q_temp);
		temp = STRINGtoINTEGER(value);
		if(isOperator(getSTRING(value)) && !isdigit(getINT(temp))) {
      //printf("found an operator!\n");
			op2 = getINT(pop(stack));
			op1 = getINT(pop(stack));
			result = evaluateOperation(getSTRING(value),op1, op2);
      //printf("Result of evaluating operation is %d\n",result);
			push(stack, newINT(result));
      //printf("Stack directly after pushing is:\n");
      //displaySTACK(stdout,stack);
      //printf("\n");
		} else if(isdigit(getINT(temp))) {
			push(stack, temp);
		} else { // It's a variable!
			//temp = findBST(bst, value);
			push(stack, temp);
		}
	} // end while

  //printf("Stack directly before popping final result is:\n");
  //displaySTACK(stdout,stack);
  //printf("\n");
	int final_result = getINT(pop(stack));
	return final_result;
}

static void setValues(QUEUE *postfix_queue,DA *columns,DA *values) {
  char *col_name = NULL;
  for(int x = 0; x < sizeDA(columns); x++) {
    col_name = getSTRING(getDA(columns,x));
    setValue(postfix_queue,col_name,getINT(getDA(values,x)));
  }
}

static void setValue(QUEUE *postfix_queue,char *col_name,int value) {
  QUEUE *temp = newQUEUE(displaySTRING);
  int size = sizeQUEUE(postfix_queue);
  STRING *queue_name = NULL;
  for(int x = 0; x < size; x++) {
    queue_name = dequeue(postfix_queue);
    if(strcmp(getSTRING(queue_name),col_name) == 0) {
      queue_name = INTEGERtoSTRING(newINT(value));
    }
    enqueue(temp,queue_name);
  }

  for(int x = 0; x < size; x++) {
    enqueue(postfix_queue,dequeue(temp));
  }
}
/** Checks if the character is an operator
  *
  * @param = The character to be checked
  */
static int isOperator(char *op) {
	if(strcmp(op,"<") == 0 || strcmp(op,">") == 0 || strcmp(op,"<=") == 0 ||
     strcmp(op,">=") == 0 || strcmp(op,"=") == 0 ||strcmp(op,"<>") == 0 ||
     strcmp(op,"AND") == 0 || strcmp(op,"OR") == 0) {
		return 1;
	}
	else return 0;
}
/** Evaluates the operation;
  * Used in calculatePostfix
  *
  * @param op = The operation
  * @param x = operand 1
  * @param y = operand 2
  */
static int evaluateOperation(char *op,int x,int y) {
  //printf("In evaluate expression. Expression is %d %s %d\n",x,op,y);
	if(strcmp(op,"<") == 0)
		return (x < y ? 1 : 0);
	else if(strcmp(op,"<=") == 0)
		return (x <= y ? 1 : 0);
	else if(strcmp(op,">") == 0)
		return (x > y ? 1 : 0);
	else if(strcmp(op,">=") == 0)
		return (x >= y ? 1 : 0);
	else if(strcmp(op,"=") == 0)
		return (x == y ? 1 : 0);
	else if(strcmp(op,"<>") == 0)
		return (x != y ? 1 : 0);
  else if(strcmp(op,"AND") == 0)
    return (x && y ? 1 : 0);
  else if(strcmp(op,"OR") == 0)
    return (x || y ? 1 : 0);
  else
    return 0;
}
/** Checks if the token is a number
  * "A number will start with a ., a minus sign, or a digit"
  *
  *  @param token = The char* to be checked
  */
extern int isNumber(char *token) {
	if(isdigit(*token))
		return 1;
	else if(*token == '.')
		return 1;
	else if(*token == '-' && isdigit(token[1]))
		return 1;
	return 0;
}

/**
  * Evaluates the priority of an operator.
  * Based off of postfix operations.
  *
  * @param = An operator
  */
static int priority(char *op) {
	if(strcmp(op,"<") == 0)
		return 0;
	else if(strcmp(op,"<=") == 0)
		return 1;
	else if(strcmp(op,">") == 0)
		return 2;
	else if(strcmp(op,">=") == 0)
		return 3;
	else if(strcmp(op,"=") == 0)
		return 4;
	else if(strcmp(op,"<>") == 0)
		return 5;
	else if(strcmp(op,"AND") == 0)
		return -1;
  else if(strcmp(op,"OR") == 0)
    return -2;
  else
    return -3;
}

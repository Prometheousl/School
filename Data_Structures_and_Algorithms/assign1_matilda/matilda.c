/**
*   Author: Alex Lay
*   Date: 9/20/2017
*	Usage: Reads in a series of variable declarations
*   as well as a mathematical expression from a file.
*   It prints on the stdout the result of the expression.
*
*  It also handles these four options:
*  
*  -v	matilda -v	give author's name and exit
*  -i	matilda -i FILENAME	print the original input to evaluating the expression
*  -p	matilda -p FILENAME	print the postfix conversion of the last infix expression
*  before evaluating the expression
*  -b	matilda -b FILENAME	print the BST holding variable values
*  before evaluating the expression
*
*/

// TODO - Restructure main function
// Debug error in CDA file with Queues

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <math.h>
#include "string.h"
#include "scanner.h"
#include "int.h"
#include "bst.h"
#include "stack.h"
#include "queue.h"
#include "real.h"
/* options */
char* Author = "Alex Lay";  /* option -v  */
char* FileName = 0;			/* Filename. Used in options
							   -i, -p, and -b */
int PrintOriginalInput = 0;	/* option -i  */
int PrintPostFix = 0;		/* option -p  */
int PrintBST = 0;			/* option -b  */

/* list of private functions used
 * in matilda.c */
static QUEUE *readFile(char *file);
static BST *parseVariables(QUEUE *tokens);
static QUEUE *parseExpression(QUEUE *tokens);
static void checkVariables(QUEUE *tokens,BST *variables);
static QUEUE *infixToPostfix(QUEUE *infix_queue);
static void printResult(BST *variables,QUEUE *infix_queue,QUEUE *postfix_queue);
static double calculatePostfix(QUEUE *q, BST *bst);
static QUEUE *copyQUEUE(QUEUE *queue);
static REAL *STRINGtoREAL(STRING *s);
static int isOperator(char);
static double evaluateOperation(char,double,double);
static int processOptions(int,char **);
static void Fatal(char *,...);
static int priority(char op);
static int isNumber(char *token);

static void displayPair(FILE *fp,void *key,void *value);

/** The main function reads in a series of variable declarations
 * as well as a mathematical expression from a file.
 * It prints on the stdout the result of the expression.
 *
 * There are 4 options the function processes:
 * -v	matilda -v	give author's name and exit
 * -i	matilda -i FILENAME	print the original input to evaluating the expression
 * -p	matilda -p FILENAME	print the postfix conversion of the last infix expression
 * before evaluating the expression
 * -b	matilda -b FILENAME	print the BST holding variable values
 * before evaluating the expression
 *
 * Variables are stored in a BST for later reference and the postfix
 * version of the expression is stored in a qeueue.
*/
int main(int argc,char **argv) {
    int argIndex;

    if (argc == 1) Fatal("%d arguments!\n",argc-1);

    argIndex = processOptions(argc,argv);
	if(argIndex != argc) {
		FileName = argv[argIndex];
	}
	else Fatal("You must enter file name!\n");
	
	QUEUE *tokens = readFile(FileName); 		// Read file into a queue
	QUEUE *original_input = copyQUEUE(tokens);  // Copy to original_input queue
	BST *variables = parseVariables(tokens);	// Read all variables into a BST
	// tokens is now the infix queue
	checkVariables(tokens,variables);			// Make sure all variables in expression are in BST
	QUEUE *postfix_queue = infixToPostfix(tokens);
	
	printResult(variables,original_input,postfix_queue);

	// Free memory
	//freeQUEUE(tokens);
	//freeQUEUE(original_input);
	//freeQUEUE(postfix_queue);
	//freeBST(variables);
    return 0;
}
/** Reads in the file specified into a queue
  *
  * @param FileName = The name of the file
  */
static QUEUE *readFile(char *FileName) {
	FILE *calcs = fopen(FileName,"r");
	QUEUE *tokens = newQUEUE(displaySTRING);
	// Read token first
	char *token = readToken(calcs);
	STRING *token_str = newSTRING(token);
	while(feof(calcs) != 1) {
		enqueue(tokens, token_str);
		//freeSTRING(token_str);
		token_str = newSTRING(readToken(calcs));
	}
	int successfulClose = fclose(calcs);
	//freeSTRING(token_str);
	return tokens;
}
/** Reads in all the variable declarations
  *	into a BST
  *
  * @param tokens = The queue of the file
  */
static BST *parseVariables(QUEUE *tokens) {
	if(sizeQUEUE(tokens) == 0) return 0;
	BST *variables = newBST(displayPair,compareSTRING);
	STRING *variable;
	STRING *equal_sign;
	REAL *value;
	STRING *token;
	STRING *var = newSTRING("var");
	while(compareSTRING(peekQUEUE(tokens),var) == 0) { // Append variable to BST
		dequeue(tokens); // "var"
		variable = dequeue(tokens);
		dequeue(tokens); // "="
		value = STRINGtoREAL(dequeue(tokens));
		dequeue(tokens); // ";"
		insertBST(variables,variable,value);
	}
	// Free memory
	free(var);
	return variables;
}
/** Checks if all variables in the expression
  * are in the BST.
  *
  * @param tokens = The infix queue
  * @param variables = The BST of variables
  */
static void checkVariables(QUEUE *tokens,BST *variables) {
	QUEUE *infix = copyQUEUE(tokens);
	STRING *s;
	while(sizeQUEUE(infix) != 0) {
		s = dequeue(infix);
		if(isalpha(*getSTRING(s))) {
			REAL *bstValue = findBST(variables,s);
			if(bstValue == NULL) {
				printf("variable %s was not declared\n",getSTRING(s));
				exit(0);
			}
		}
	}
	//freeQUEUE(infix);
}
/** Converts the infix queue to the postfix queue
  * 
  * @param infix_queue = The infix queue
  */
static QUEUE *infixToPostfix(QUEUE *infix_queue) {
	STACK *operators = newSTACK(displaySTRING);
	QUEUE *postfix_queue = newQUEUE(displaySTRING);
	STRING *token = dequeue(infix_queue);
	STRING *semicolon = newSTRING(";");
	while(compareSTRING(token,semicolon) != 0) {
		if(isalpha(*getSTRING(token)) || isNumber(getSTRING(token))) { 
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
				push(operators,token);
			}
			else if(isOperator(*getSTRING(token))){
				while(sizeSTACK(operators) != 0 && priority(*getSTRING(peekSTACK(operators))) >= priority(*getSTRING(token))) {
					enqueue(postfix_queue,pop(operators));
				}
				push(operators,token);
			}
		}
		
		token = dequeue(infix_queue);
	} // end while
	// Append the rest of the operators to the postfix_queue
	while(sizeSTACK(operators) != 0) {
		enqueue(postfix_queue,pop(operators));
	}
	// Free memory
	//freeSTACK(operators);
	free(semicolon);
	return postfix_queue;
}
/**
 *  Prints the -i,-p, and -b options.
 *  Also prints the result of the calculation.
 *
 *  If multiple options are given, 
 *	print the -i output before the -p output 
 *	and print the -p output before the -b output. 
 *	Those options can come in any order.
 */
static void printResult(BST *variables,QUEUE *infix_queue,QUEUE *postfix_queue) {
	if(PrintOriginalInput == 1) { // -i
		while(sizeQUEUE(infix_queue) != 0) {
			char *input = getSTRING(dequeue(infix_queue));
			printf("%s",input);
			if(*input != ';')
				printf(" ");
			else
				printf("\n");
		}
	}
	
	if(PrintPostFix == 1) { // -p
		QUEUE *temp = copyQUEUE(postfix_queue);
		STRING *value;
		while(sizeQUEUE(temp) != 0) {
			value = dequeue(temp);
			printf("%s",getSTRING(value));
			if(sizeQUEUE(temp) != 0)
				printf(" ");
			else
				printf("\n");
		}
	}
	
	if(PrintBST == 1) { // -b
		displayBST(stdout,variables);
		printf("\n");
	}
	
	// Print Result
	double result = calculatePostfix(postfix_queue,variables);
	printf("%f\n",result);
}
/**
 * Takes the postfix qeueue and evaluates it.
 * 
 * It does this by pushing values to a stack
 * and evaluating operators when they appear.
 *
 * @param postfix_queue = A postfix queue
 * @param bst = A bst filled with variables
 */
static double calculatePostfix(QUEUE *postfix_queue,BST *bst) {
	double result = 0;
	double x, y;
	int a,b;
	STRING *value;
	REAL *temp;
	STACK *stack = newSTACK(displayREAL);
	while(sizeQUEUE(postfix_queue) != 0) {
		value = dequeue(postfix_queue);
		temp = STRINGtoREAL(value);
		if(isOperator(*getSTRING(value)) && !isdigit(getSTRING(value)[1])) {
			double op2 = getREAL(pop(stack));
			double op1 = getREAL(pop(stack));
			double result = evaluateOperation(*getSTRING(value),op1, op2);
			push(stack, newREAL(result));
		} else if(isNumber(getSTRING(value))) {
			push(stack, temp);
		} else { // It's a variable!
			temp = findBST(bst, value);
			push(stack, temp);
		}
	} // end while

	double final_result = getREAL(pop(stack));
	// Free memory
	//free(temp);
	//freeSTACK(stack);
	return final_result;
}
/** Prints a key/value pair to fp; used in BST
  * 
  * For example if key = "a" and
  * value = "47.2" it prints:
  * 		a=47.2
  */
static void displayPair(FILE *fp,void *key,void *value) {
	STRING *key_str = key;
	REAL *value_real = value;
	fprintf(fp,"%s=%f",getSTRING(key_str),getREAL(value_real));
}
/** Copies the queue and returns it
  *
  * @param queue = The queue to be copied
  */
static QUEUE *copyQUEUE(QUEUE *queue) {
	// TODO- Figure out how to "deep copy" queue
	QUEUE *new_queue = newQUEUE(displaySTRING);
	QUEUE *temp = newQUEUE(displaySTRING);
	while(sizeQUEUE(queue) != 0) {
		STRING *s = dequeue(queue);
		enqueue(new_queue,s);
		enqueue(temp,s);
	}
	while(sizeQUEUE(temp) != 0) {
		enqueue(queue,dequeue(temp));
	}
	//freeQUEUE(temp);
	return new_queue;
}
/** Converts a STRING struct to a REAL struct
  *
  * @param s = The STRING to be converted
  */
static REAL *STRINGtoREAL(STRING *s) {
	double value = atof(getSTRING(s));
	REAL *temp = newREAL(value);
	return temp;
}
/** Checks if the character is an operator
  *
  * @param = The character to be checked
  */
static int isOperator(char op) {
	if(op == '+' || op == '-' || op == '*' || op == '/' || op == '%' || op == '^') {
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
static double evaluateOperation(char op,double x,double y) {
	double result;
	if(op == '+')
		result = x + y;
	else if(op == '-')
		result = x - y;
	else if(op == '*')
		result = x * y;
	else if(op == '/')
		result = x / y;
	else if(op == '%')
		result = fmod(x,y);
	else if(op == '^')
		result = pow(x,y);
	return result;
}
/** Checks if the token is a number
  * "A number will start with a ., a minus sign, or a digit"
  *
  *  @param token = The char* to be checked
  */
static int isNumber(char *token) {
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
static int priority(char op) {
	if(op == '(')
		return 0;
	else if(op == '+')
		return 1;
	else if(op == '-')
		return 2;
	else if(op == '*')
		return 3;
	else if(op == '/')
		return 4;
	else if(op == '%')
		return 5;
	else if(op == '^')
		return 6;
}
/**
 * Prints to the standard error
 * if an error occurred
 */
static void Fatal(char *fmt, ...) {
    va_list ap;

    fprintf(stderr,"An error occured: ");
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    exit(-1);
}
/**
 * Processes options from the standard input
 *
 * only -oXXX  or -o XXX options
 */
static int processOptions(int argc, char **argv) {
    int argIndex;
    int argUsed;
    int separateArg;
    char *arg;

    argIndex = 1;

    while (argIndex < argc && *argv[argIndex] == '-') {
        /* check if stdin, represented by "-" is an argument */
        /* if so, the end of options has been reached */
        if (argv[argIndex][1] == '\0') return argIndex;

        separateArg = 0;
        argUsed = 0;

        if (argv[argIndex][2] == '\0') {
            arg = argv[argIndex+1];
            separateArg = 1;
        }
        else
            arg = argv[argIndex]+2;

        switch (argv[argIndex][1]) {
            /*
             * when option has an argument, do this
             *
             *     examples are -m4096 or -m 4096
             *
             *     case 'm':
             *         MemorySize = atol(arg);
             *         argUsed = 1;
             *         break;
             *
             *
             * when option does not have an argument, do this
             *
             *     example is -a
             *
             *     case 'a':
             *         PrintActions = 1;
             *         break;
             */

			case 'v':
				printf("%s\n", Author);
				exit(0);
				break;
			case 'i':
				PrintOriginalInput = 1;
				break;
			case 'p':
				PrintPostFix = 1;
				break;
			case 'b':
				PrintBST = 1;
				break;
            default:
                Fatal("option %s not understood\n",argv[argIndex]);
        }

        if (separateArg && argUsed)
            ++argIndex;

        ++argIndex;
    }

    return argIndex;
}
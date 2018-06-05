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
static QUEUE *copyQUEUE(QUEUE *queue);
static QUEUE *appendToQUEUE(QUEUE *dest,QUEUE *src);
static REAL *STRINGtoREAL(STRING *s);
static int isOperator(char);
static double evaluateOperation(char,double,double);
static int processOptions(int,char **);
static void Fatal(char *,...);
static char *concatenateStrings(char *,char *);
static int priority(char op);
static double calculatePostfix(QUEUE *q, BST *bst);
static char *addToOriginalInput(char *originalInput,char *token);
static int isNumber(char *token);
static int string_length(char *s);

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
	
	// Read file into a queue
	printf("About to read in file\n");
	QUEUE *tokens = readFile(FileName);
	displayQUEUE(stdout,tokens);
	printf("\n");
	// Copy to original_input queue
	QUEUE *original_input = copyQUEUE(tokens);
	printf("Original input queue is:\n");
	displayQUEUE(stdout,original_input);
	printf("\n");
	// Read all variables into a BST
	printf("About to read in variables\n");
	BST *variables = parseVariables(tokens);
	printf("Queue after parsing variables (infix)");
	displayQUEUE(stdout,tokens);
	printf("\n");
	// tokens is now the infix queue
	// Make sure all variables in expression are in BST
	checkVariables(tokens,variables);
	// convert infix to postfix
	QUEUE *postfix_queue = infixToPostfix(tokens);
	printf("Postfix queue after infixToPostfix:\n");
	displayQUEUE(stdout,postfix_queue);
	printf("\n");

    /* If multiple options are given, 
	print the -i output before the -p output 
	and print the -p output before the -b output. 
	Those options can come in any order. */
	
	printResult(variables,original_input,postfix_queue);

	// TODO - free memory
	freeQUEUE(tokens);
	freeQUEUE(original_input);
	freeQUEUE(postfix_queue);
	freeBST(variables);
    return 0;
}
/** Reads in the file specified into a queue
  */
static QUEUE *readFile(char *FileName) {
	FILE *calcs = fopen(FileName,"r");
	QUEUE *tokens = newQUEUE(displaySTRING);
	
	// As Lusth said, read token in first
	char *token = readToken(calcs);
	STRING *token_str = newSTRING(token);
	while(feof(calcs) != 1) {
		//printf("Adding %s to tokens\n",token);
		enqueue(tokens, token_str);
		//freeSTRING(token_str);
		token_str = newSTRING(readToken(calcs));
	}
	int successfulClose = fclose(calcs);
	//freeSTRING(token_str);
	return tokens;
}
/** Reads in all the variable declarations
	into a BST
  */
static BST *parseVariables(QUEUE *tokens) {
	if(sizeQUEUE(tokens) == 0) return 0;
	BST *variables = newBST(displayPair,compareSTRING);
	STRING *variable;
	STRING *equal_sign;
	REAL *value;
	STRING *token;
	STRING *var = newSTRING("var");
	//printf("Initial token is %s\n",getSTRING(token));
	while(compareSTRING(peekQUEUE(tokens),var) == 0) { // Append variable to BST
		//printf("Inside while loop\n");
		dequeue(tokens); // "var"
		variable = dequeue(tokens);
		dequeue(tokens); // "="
		value = STRINGtoREAL(dequeue(tokens));
		dequeue(tokens); // ";"
		//printf("Inserting into BST:\n");
		//printf("%s = %f\n",getSTRING(variable),getREAL(value));
		insertBST(variables,variable,value);
	}
	// Free memory
	free(var);
	return variables;
}
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
	freeQUEUE(infix);
}
/**
 *
 */
static QUEUE *infixToPostfix(QUEUE *infix_queue) {
	printf("In infixtoPostfix\n");
	STACK *operators = newSTACK(displaySTRING);
	QUEUE *postfix_queue = newQUEUE(displaySTRING);
	STRING *token = dequeue(infix_queue);
	STRING *semicolon = newSTRING(";");
	while(compareSTRING(token,semicolon) != 0) {
		printf("Infix queue:\n");
		displayQUEUE(stdout,infix_queue);
		printf("\n");
		printf("Size of QUEUE is %d\n",sizeQUEUE(postfix_queue));
		printf("Postfix QUEUE:\n");
		displayQUEUE(stdout,postfix_queue);
		printf("\n");
		printf("Token is: %s\n",getSTRING(token));
		
		printf("Size of queue prior to if statements is %d\n",sizeQUEUE(postfix_queue));
		if(isalpha(*getSTRING(token)) || isNumber(getSTRING(token))) { 
			// Append variable/number to postfix queue
			printf("Appending %s to postfix queue\n",getSTRING(token));
			enqueue(postfix_queue,token);
		}
		else { // token is an operator
			if(*getSTRING(token) == ')') { // Append all operators to postfix_queue until '(' found
				STRING *x = pop(operators);
				while(sizeSTACK(operators) != 0 && *getSTRING(x) != '(') {
					printf("Popping stack. Popped value is %s\n",getSTRING(x));
					enqueue(postfix_queue,x);
					//displayQUEUE(stdout,postfix_queue);
					x = pop(operators);
				}
			}
			else if(sizeSTACK(operators) == 0 || *getSTRING(token) == '(' || *getSTRING(peekSTACK(operators)) == '('){
				printf("Stack is empty /has a left parenthases\n");
				printf("Pushing %s to the stack\n",getSTRING(token));
				push(operators,token);
			}
			else if(isOperator(*getSTRING(token))){
				printf("Size of queue at top of isOperator is %d\n",sizeQUEUE(postfix_queue));
				printf("Priority of top of stack (%c)",*getSTRING(peekSTACK(operators)));
				printf(" is %d\n",priority(*getSTRING(peekSTACK(operators))));
				printf("Priority of token is (%c)",*getSTRING(token));
				printf(" is %d\n",priority(*getSTRING(token)));
				while(sizeSTACK(operators) != 0 && priority(*getSTRING(peekSTACK(operators))) >= priority(*getSTRING(token))) {
					printf("Appending operator %s to queue\n",getSTRING(peekSTACK(operators)));
					enqueue(postfix_queue,pop(operators));
				}
				printf("Priority of new operator is < operator on top of stack so appending it to stack\n");
				printf("Postfix queue size prior to push of isOperator is %d\n",sizeQUEUE(postfix_queue));
				push(operators,token);
				printf("Postfix queue size at bottom of isOperator is %d\n",sizeQUEUE(postfix_queue));
			}
		}
		
		token = dequeue(infix_queue);
		printf("Postfix queue size at bottom of loop is %d\n",sizeQUEUE(postfix_queue));
	} // end while
	// Append the rest of the operators to the postfix_queue
	while(sizeSTACK(operators) != 0) {
		enqueue(postfix_queue,pop(operators));
	}
	// Free memory
	freeSTACK(operators);
	free(semicolon);
	return postfix_queue;
}
/**
 *
 */
static void printResult(BST *variables,QUEUE *infix_queue,QUEUE *postfix_queue) {
	printf("In print result\n");
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
 * Prints a key/value pair to fp
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
	freeQUEUE(temp);
	return new_queue;
}
/**
 *
 */
static QUEUE *appendToQUEUE(QUEUE *dest,QUEUE *src) {
	while(sizeQUEUE(src) != 0) {
		enqueue(dest,dequeue(src));
	}
	return dest;
}
/**
 *
 */
static REAL *STRINGtoREAL(STRING *s) {
	double value = atof(getSTRING(s));
	REAL *temp = newREAL(value);
	return temp;
}
/**
 *
 */
static int isOperator(char op) {
	if(op == '+' || op == '-' || op == '*' || op == '/' || op == '%' || op == '^') {
		return 1;
	}
	else return 0;
}
/**
 *
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
/**
 * Concatenates the token string to the original
 * input string. Also adds whitespace.
 *
 * It uses the concatenateStrings function.
 */
static char *addToOriginalInput(char *originalInput,char *token) {
	concatenateStrings(originalInput,token);
	if(token[0] != ';')
		concatenateStrings(originalInput," ");
	else
		concatenateStrings(originalInput,"\n");
	return originalInput;
}
/**
 * Takes the postfix qeueue and evaluates it.
 * 
 * It does this by pushing values to a stack
 * and evaluating operators when they appear.
 */
static double calculatePostfix(QUEUE *postfix_queue,BST *bst) {
	printf("In calculate\n");
	double result = 0;
	double x, y;
	int a,b;
	STRING *value;
	REAL *temp;
	STACK *stack = newSTACK(displayREAL);
	displayQUEUE(stdout,postfix_queue);
	printf("\n");
	printf("The size of the postfix queue is: %d\n",sizeQUEUE(postfix_queue));
	while(sizeQUEUE(postfix_queue) != 0) {
		value = dequeue(postfix_queue);
		REAL *r = STRINGtoREAL(value);
		printf("value is %s\n",getSTRING(value));
		if(isOperator(*getSTRING(value)) && !isdigit(getSTRING(value)[1])) {
			printf("Value is an operator: %c\n",*getSTRING(value));
			double op2 = getREAL(pop(stack));
			double op1 = getREAL(pop(stack));
			printf("Performing operation %f %c %f = ",x,*getSTRING(value),y);
			double result = evaluateOperation(*getSTRING(value),op1, op2);
			printf("%f\n",result);
			push(stack, newREAL(result));
		} else if(isNumber(getSTRING(value))) {
			push(stack, r);
		} else {	//Should be a variable
			r = findBST(bst, value);
			push(stack, r);
		}
	} // end while
	printf("Size of stack should be 1 now.\n");
	printf("Size of stack is %d\n",sizeSTACK(stack));
	double final_result = getREAL(pop(stack));
	// Free memory
	//free(temp);
	//freeSTACK(stack);
	return final_result;
}
/**
 *
 * a number will start with a ., a minus sign, or a digit
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
 *
 * Based off of postfix operations.
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
 * Concatenates two strings.
 *
 * I had to write this function myself
 * because my compiler has a problem with
 * me including <string.h> for some reason
 */
static char* concatenateStrings(char *str1, char *str2) {
	char *ret = str1;
	while(*str1)
		str1++;
	while(*str1++ = *str2++)
		;
	//printf("Str1 is now %s.\n",ret);
	return ret;
}

static int string_length(char *s) {
   int c = 0;
 
   while(*(s+c))
      c++;
 
   return c;
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
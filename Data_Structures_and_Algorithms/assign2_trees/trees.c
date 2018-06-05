#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include "bst.h"
#include "rbt.h"
#include "gt.h"
#include "queue.h"
#include "string.h"
#include "scanner.h"

/* options */
int green = 0;    /* option -s      */
int red_black = 0;     /* option -n XXXX */
char *Author = "Alex Lay";
// *** Static functions ***
static QUEUE *readCommands(char *);
static GT *corpusToGreenTree(char *);
static RBT *corpusToRBTree(char *);
static void processGTcommands(GT *,QUEUE *);
static void processRBTcommands(RBT *,QUEUE *);
static STRING *readNode(FILE *);
static STRING *cleanString(STRING *);
static STRING *STRINGtoLower(STRING *);
static STRING *stripNonAlpha(STRING *);
static void removeChar(char *, char);
static STRING *cleanWhiteSpace(STRING *);
static STRING *trimWhiteSpace(STRING *);
static STRING *multiSpaceToSingleSpace(STRING *str);

static int processOptions(int,char **);
void Fatal(char *,...);

/** Reads in a data file and a commands file.
  * It then converts the data file or "corpus"
  * into a green tree or red black tree depending
  * on if the -g or -r option is specified.
  *
  * It then reads a list of commands to perform
  * on the tree into a queue
  * and then executes the commands on either
  * the green or red black tree.
  *
  * Example invocation:
  *  	$ trees -g data commands
  */
int main(int argc,char **argv) {
    int argIndex;
	char *data_file_name;
	char *commands_file_name;
	char *output_file_name = NULL;

    if (argc == 1) Fatal("%d arguments!\n",argc-1);
    argIndex = processOptions(argc,argv);

	if(green == 0 && red_black == 0)
		red_black = 1;
    if (argIndex == argc)
        printf("No corpus specified\n");
    else {
		data_file_name = argv[argIndex];
		argIndex++;
		if(argIndex == argc)
			printf("No commands specified\n");
		else {
			commands_file_name = argv[argIndex];
			argIndex++;
		}
		if(argIndex != argc)
			output_file_name = argv[argIndex];
    }

	QUEUE *commands = readCommands(commands_file_name);
	if(green) { // Read data into a green tree
		GT *green_tree = corpusToGreenTree(data_file_name);
		processGTcommands(green_tree,commands);
	}
	else if(red_black) { // Read data into a red-black tree
		RBT *rb_tree = corpusToRBTree(data_file_name);
		processRBTcommands(rb_tree,commands);
	}
	
    return 0;
}
/** Reads the command file into a queue
  */
static QUEUE *readCommands(char *commands_file_name) {
	QUEUE *queue = newQUEUE(displaySTRING);
	FILE *commands = fopen(commands_file_name,"r");
	// Read in either a phrase or token
	STRING *str = readNode(commands);
	while(!feof(commands)) {
		enqueue(queue,str);
		str = readNode(commands);
	}
	int successfulClose = fclose(commands);
	return queue;
}
/** Converts the corpus into a green tree
  */
static GT *corpusToGreenTree(char *data_file_name) {
	GT *gt = newGT(displaySTRING,comparatorSTRING);
	FILE *data = fopen(data_file_name,"r");
	STRING *node_str = readNode(data);
	while(!feof(data)) {
		if(node_str != NULL)
			insertGT(gt,node_str);
		node_str = readNode(data);
	}
	int successfulClose = fclose(data);
	return gt;
}
/** Converts the corpus into a red black tree
  */
static RBT *corpusToRBTree(char *data_file_name) {
	RBT *rbt = newRBT(displaySTRING,comparatorSTRING);
	FILE *data = fopen(data_file_name,"r");
	STRING *node_str = readNode(data);
	while(!feof(data)) {
		if(node_str != NULL) {
			insertRBT(rbt,node_str);
		}
		node_str = readNode(data);
	}
	int successfulClose = fclose(data);
	return rbt;
}
/** Executes the operations on the green tree 
  * given from the commands
  *
  * @param q = The queue of commands
  */
static void processGTcommands(GT *gt,QUEUE *q) {
	int frequency = 0;
	STRING *str = NULL;
	while(sizeQUEUE(q) != 0) {
		str = dequeue(q);
		switch(*getSTRING(str)) {
			case 'i':
				str = dequeue(q);
				insertGT(gt,str);
				break;
			case 'd':
				str = dequeue(q);
				deleteGT(gt,str);
				break;
			case 'f':
				str = dequeue(q);
				frequency = findGT(gt,str);
				printf("Frequency of %s: %d\n",getSTRING(str),frequency);
				break;
			case 's':
				displayGT(stdout,gt);
				break;
			case 'r':
				statisticsGT(stdout,gt);
				break;
		}
	}
}
/** Executes the operations on the red black tree 
  * given from the commands
  *
  * @param q = The queue of commands
  */
static void processRBTcommands(RBT *rbt,QUEUE *q) {
	int frequency = 0;
	STRING *str = NULL;
	while(sizeQUEUE(q) != 0) {
		str = dequeue(q);
		switch(*getSTRING(str)) {
			case 'i':
				str = dequeue(q);
				insertRBT(rbt,str);
				break;
			case 'd':
				str = dequeue(q);
				deleteRBT(rbt,str);
				break;
			case 'f':
				str = dequeue(q);
				frequency = findRBT(rbt,str);
				printf("Frequency of %s: %d\n",getSTRING(str),frequency);
				break;
			case 's':
				displayRBT(stdout,rbt);
				break;
			case 'r':
				statisticsRBT(stdout,rbt);
				break;
		}
	}
}
/** Reads a "node" from a file.
  * This node can either be a token
  * or a phrase.
  *
  * The function then cleans the node.
  */
static STRING *readNode(FILE *data) {
	STRING *node_str = NULL;
	if(stringPending(data))
		node_str = newSTRING(readString(data));
	else
		node_str = newSTRING(readToken(data));
	// Convert all characters to lower case
	if(getSTRING(node_str)) // If node_str != NULL
		node_str = cleanString(node_str);
	return node_str;
}
/** This function converts all characters
  * to lower case,
  * removes excess whitespace, and
  * strips all non-alpha characters from
  * the string.
  * If after all this, the string is empty
  * it returns NULL.
  */
static STRING *cleanString(STRING *str) {
	str = stripNonAlpha(str);
	str = STRINGtoLower(str);
	str = cleanWhiteSpace(str);
	return str;
}
/** Strips all characters that are not
  * alpha from the string.
  * (Uses the isalpha function)
  */
static STRING *stripNonAlpha(STRING *string) {
	char *str = getSTRING(string);
	size_t length = strlen(str);
	char *dest = str;
	// Need to store a pointer to the beginning of the array
	char *pointer_to_beginning = dest;
	while(*str != '\0') {
		while(*str != '\0' && !isalpha(*str) && !isspace(*str)) {
			str++;
		}
		// Copy from source to dest
		*dest++ = *str++;
	}
	*dest = '\0';
	string = newSTRING(pointer_to_beginning);
	return string;
}
/** Removes all the occurrences of a
*	char from the given string
*
*	@param str = The string to be removed from
*	@param garbage = The character to remove
*/
static void removeChar(char *str, char garbage) {
	char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}
/** Converts a STRING to all lower case
  */
static STRING *STRINGtoLower(STRING *str) {
	char *p = getSTRING(str);
	size_t length = strlen(p);
	char *lower = (char *)malloc(length + 1); // Plus one for null terminator
	lower[length] = 0; // Last byte = null terminator
	// copy all char bytes to the new buffer
	for(int i = 0; i < length; i++) {
		lower[i] = tolower(p[i]);
	}
	STRING *lower_str = newSTRING(lower);
	return lower_str;
}
/** Cleans up a STRING's leading, trailing,
  * and excess whitespace in the middle
  */
static STRING *cleanWhiteSpace(STRING *str) {
	// Remove leading & trailing whitespace
	str = trimWhiteSpace(str);
	if(str == NULL) // Phrase is all spaces
		return NULL;
	// Replace multiple spaces w/one space
	str = multiSpaceToSingleSpace(str);

	return str;
}
/** Trims the leading and trailing whitespace
  */
static STRING *trimWhiteSpace(STRING *string) {
	char *str = getSTRING(string);
	char *end;
	// Trim leading space
	while(isspace(*str))
		str++;
	if(*str == 0) // All spaces (baaaad)
		return NULL;
	// Trim trailing space
	end = str + strlen(str) - 1; // Minus \0
	while(end > str && isspace(*end))
		end--;
	// Append the null terminator
	*(end + 1) = 0;
	
	string = newSTRING(str);
	return string;
}
/** Converts all the multiple spaces in the middle
  * of the string into one space
  */
static STRING *multiSpaceToSingleSpace(STRING *string) {
	char *str = getSTRING(string);
	size_t length = strlen(str);
	char *dest = (char *)malloc(sizeof(char) * length + 1);
	int dest_index = 0;
	dest[dest_index] = str[0];
	dest_index++;
	for(int i=1;  str[i] != '\0'; i++){
		if(str[i] == '\n' || str[i] == '\t') {
			str[i] = ' ';
		}
		if(str[i-1] == ' '){
			while(isspace(str[i])) {
				i++;
			}
		}
		dest[dest_index] = str[i];
		dest_index++;
	}
	dest[dest_index] = '\0'; // Make sure it terminates (otherwise gets junk in its trunk)
	string = newSTRING(dest);
	return string;
}
void Fatal(char *fmt, ...) {
    va_list ap;

    fprintf(stderr,"An error occured: ");
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    exit(-1);
}

/* only -oXXX  or -o XXX options */

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
            case 'g':
                green = 1;
                break;
            case 'r':
                red_black = 1;
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
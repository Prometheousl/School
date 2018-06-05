/*
 * stack.c
 *
 *  Created on: Sep 7, 2017
 *      Author: alexl
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"
#include "da.h"

struct da {
	void **data;
	int size;
	int capacity;
	void (*display)(FILE *,void *);
};

struct stack {
	DA *da;
	void (*display)(FILE *,void *);
};
/**
 * The constructor is passed a function that knows how to
 * display the generic value stored in the stack.
 * That function is stored in a display field of the STACK object.
 *
 * @param d = The display function
 * @return The new stack
 */
extern STACK *newSTACK(void (*display)(FILE *,void *)) {
	STACK *stack = malloc(sizeof(STACK));
	assert(stack != 0);
	stack->da = newDA(display);
	stack->display = display;
	return stack;
}
/**
 * The push method runs in constant or amortized constant time.
 * The value to be pushed is stored in the underlying data structure.
 *
 * @param items = The stack object to be pushed to
 * @param value = The value to be pushed
 */
extern void push(STACK *items,void *value) {
	insertDA(items->da,value);
}
/**
 * The pop method runs in constant or amortized constant time.
 * The value to be popped is removed in the underlying data structure.
 *
 * @param items = The stack to be popped from
 * @return the popped value
 */
extern void *pop(STACK *items) {
	void *poppedItem = removeDA(items->da);
	return poppedItem;
}
/**
 * The peek method returns the value ready to come off the stack,
 * but leaves the stack unchanged.
 * It runs in constant time.
 *
 * @param items = The stack to be peeked at
 * @return the peeked value
 */
extern void *peekSTACK(STACK *items) {
	void *topValue = getDA(items->da,sizeSTACK(items) - 1);
	return topValue;
}
/**
 * The size method returns the number of items stored in the stack.
 * It runs in amortized constant time.
 *
 * @param items = A stack
 * @return The stack's size
 */
extern int sizeSTACK(STACK *items) {
	return sizeDA(items->da);
}
/**
 * The display method prints the items stored in the stack.
 * If the integers 5, 6, 2, 9, and 1 are pushed in the order given,
 * the method would generate this output: |1,9,2,6,5|
 * with no preceding or following whitespace.
 * An empty stack displays as ||.
 *
 * @param fp = The file to be written to
 * @param items = The stack object
 */
extern void displaySTACK(FILE *fp,STACK *items) {
	fprintf(fp,"|");
		int i = 0;
		for(i = sizeSTACK(items) - 1; i >= 0; --i) {
			items->display(fp, getDA(items->da,i));
			if(i != 0) { fprintf(fp,","); }
		}
		fprintf(fp,"|");
}
/**
  * Returns the underlying DA of the stack.
  *
  * @param items = The stack to get the DA from
  * @return = The underlying DA
  */
extern DA *getUnderlyingDA(STACK *items) {
  return items->da;
}
/**
 * The visualize method simply calls the display method of the
 * data structure upon which the stack is based.
 *
 * @param fp = The file to be written to
 * @param items = The stack object
 */
extern void visualizeSTACK(FILE *fp,STACK *items) {
	displayDA(fp,items->da);
}
/**
 * Frees the underlying DA object and STACK object
 *
 * @param items = the stack to be freed
 */
/*extern void freeSTACK(STACK *items) {
	freeDA(items->da);
	free(items);
}*/

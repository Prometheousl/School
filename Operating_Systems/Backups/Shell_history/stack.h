/*
 * stack.h
 *
 * Author: Alex Lay
 * Created on: Sep 6, 2017
 * Course: CS 201 Data Structures and Algorithms
 * Desc: This header file contains the function prototypes
 * 		 for a stack.
 * Purpose: To define function prototypes for a stack.
 */

#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>
#include "da.h"

typedef struct stack STACK;

extern STACK *newSTACK(void (*d)(FILE *,void *));
extern void push(STACK *items,void *value);
extern void *pop(STACK *items);
extern void *peekSTACK(STACK *items);
extern int sizeSTACK(STACK *items);
extern void displaySTACK(FILE *fp,STACK *items);
extern void visualizeSTACK(FILE *fp,STACK *items);
extern DA *getUnderlyingDA(STACK *items);

//extern void freeSTACK(STACK *items);

#endif /* STACK_H_ */

/*
 * File: da.h
 *
 * Author: Alex Lay
 * Created on: Sep 6, 2017
 * Course: CS 201 Data Structures and Algorithms
 * Desc: This header file contains the function prototypes
 * 		 for a dynamic array.
 * Purpose: To define function prototypes for a dynamic array.
 */

#ifndef DA_H_
#define DA_H_

#include <stdio.h>

typedef struct da DA;

extern DA *newDA(void (*display)(FILE *,void *));
extern void insertDA(DA *items,void *value);
extern void *removeDA(DA *items);
extern void unionDA(DA *recipient,DA *donor);
extern void *getDA(DA *items,int index);
extern void *setDA(DA *items,int index,void *value);
extern void **extractDA(DA *items);
extern int sizeDA(DA *items);
extern void visualizeDA(FILE *fp,DA *items);
extern void displayDA(FILE *fp,DA *items);

//extern void freeDA(DA *items);

#endif /* DA_H_ */

/*
 * real.h
 *
 *  Created on: Sep 6, 2017
 *      Author: alexl
 */

#ifndef REAL_H_
#define REAL_H_

#include <stdio.h>

typedef struct real REAL;

extern REAL *newREAL(double);
extern double getREAL(REAL *);
extern double setREAL(REAL *,double);
extern void displayREAL(FILE *,void *);

extern void freeREAL(REAL *v);

#endif /* REAL_H_ */

/*
 * int.h
 *
 *  Created on: Sep 6, 2017
 *      Author: alexl
 */

#ifndef INT_H_
#define INT_H_

#include <stdio.h>

typedef struct INT INTEGER;

extern INTEGER *newINT(int);
extern int getINT(INTEGER *);
extern int setINT(INTEGER *,int);
extern void displayINT(FILE *,void *);
extern int comparatorINT(void *, void *);

#endif /* INT_H_ */

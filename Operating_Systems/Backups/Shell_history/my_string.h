/*
 * STRING.h
 *
 *  Created on: Sep 6, 2017
 *      Author: alexl
 */

#ifndef STRING_H_
#define STRING_H_

#include <stdio.h>

typedef struct STR STRING;

extern STRING *newSTRING(char *);
extern char *getSTRING(STRING *);
extern char *setSTRING(STRING *,char *);
extern void displaySTRING(FILE *,void *);
extern int compareSTRING(void *, void *);
extern void freeSTRING(STRING *);

#endif /* STRING_H_ */

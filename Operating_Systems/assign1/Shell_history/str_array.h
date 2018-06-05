/*
 * STRING.h
 *
 *  Created on: Sep 6, 2017
 *      Author: alexl
 */

#ifndef STR_ARRAY_H_
#define STR_ARRAY_H_

#include <stdio.h>
#include "da.h"

typedef struct STR_ARRAY STR_ARRAY;

extern STR_ARRAY *newSTR_ARRAY(char **);
extern DA *getSTR_ARRAY(STR_ARRAY *);
extern void displaySTR_ARRAY(FILE *,void *);

#endif /* STR_ARRAY_H_ */

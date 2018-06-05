/*
 * File: table.h
 *
 * Author: Alex Lay
 * Created on: April 7, 2018
 */

#ifndef TABLE_H_
#define TABLE_H_

#include <stdio.h>
#include "da.h"

typedef struct functional_dependency DEP;
typedef struct table TABLE;

extern TABLE *newTABLE(char *,DA *,char *);
extern TABLE *getTABLE(TABLE *);
extern void insertDEP(TABLE *,char *,char *);
extern int determineForm(TABLE *);
extern void displayTable(FILE *,void *);
extern void freeTABLE(TABLE *);

#endif /* TABLE_H_ */

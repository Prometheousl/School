/*
 * File: table.h
 *
 * Author: Alex Lay
 * Created on: April 7, 2018
 */

#ifndef T1A_H_
#define T1A_H_

#include <stdio.h>
#include "../data_structures/da.h"

typedef struct table_a TABLEA;

extern TABLEA *newTABLEA();
extern void insertRowA(TABLEA *,int,int,int,int);
extern void printTA(FILE *,TABLEA *ta);
extern int getSizeA(TABLEA *t);
extern DA *getK1A(TABLEA *t);
extern DA *getTSA(TABLEA *t);
extern DA *getC1(TABLEA *t);
extern DA *getC2(TABLEA *t);
extern DA *getAllColumnsA(TABLEA *ta);

#endif /* TABLE_H_ */

/*
 * File: table.h
 *
 * Author: Alex Lay
 * Created on: April 7, 2018
 */

#ifndef T1B_H_
#define T1B_H_

#include <stdio.h>
#include "../data_structures/da.h"

typedef struct table_b TABLEB;

extern TABLEB *newTABLEB();
extern void insertRowB(TABLEB *,int,int,int,int,int);
extern void printTB(FILE *,TABLEB *tb);
extern int getSizeB(TABLEB *t);
extern DA *getK1B(TABLEB *t);
extern DA *getTSB(TABLEB *t);
extern DA *getC3(TABLEB *t);
extern DA *getC4(TABLEB *t);
extern DA *getC5(TABLEB *t);
extern DA *getAllColumnsB(TABLEB *tb);

#endif /* TABLE_H_ */

/*
 * File: table.h
 *
 * Author: Alex Lay
 * Created on: April 7, 2018
 */

#ifndef T2_H_
#define T2_H_

#include <stdio.h>
#include "../data_structures/da.h"

typedef struct table_2 TABLE2;

extern TABLE2 *newTABLE2();
extern void insertRow2(TABLE2 *,int,int,int,int,int);
extern void printT2(FILE *,TABLE2 *t);
extern int getSize2(TABLE2 *t);
extern DA *getK2(TABLE2 *t);
extern DA *getTS2(TABLE2 *t);
extern DA *getA1(TABLE2 *t);
extern DA *getA2(TABLE2 *t);
extern DA *getA3(TABLE2 *t);

#endif /* TABLE_H_ */
